#include "Rua.h"
#include "Packet.h"

Rua::Rua(int port): port(port)
	{
        //set unpack rules
        //length(4 bytes)+type(4 bytes)+data(length bytes)
        memset(uuid, 0, 40);
        unpack_setting_s us;
        us.mode = UNPACK_BY_LENGTH_FIELD;
        us.package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
        us.body_offset = 4;
        us.length_field_offset = 0;
        us.length_field_bytes = 4;
        us.length_adjustment = 4;
        us.length_field_coding = ENCODE_BY_BIG_ENDIAN;

        TcpServer();
        setUnpack(&us);
        int listen = createsocket(port);
        if (listen < 0)
        {
            std::cout << "binding error" << std::endl;
            return;
        }
        std::cout << "established@" << port << std::endl;
        onConnection = [](const SocketChannelPtr& ch)
        {
            if (ch->isConnected())
            {
                printf("Client connected from %s\n", ch->peeraddr().c_str());
            }
            else
            {
                printf("Client disconnected from %s\n", ch->peeraddr().c_str());
            }
        };
        onMessage = [&](const SocketChannelPtr& ch, Buffer* buf)
        {
            std::cout << "This message is of size " << buf->len << std::endl;
            Packet packet(buf);
            StreamBuilder sb;
            switch (packet.type)
            {
            case(PacketType::PREREGISTER_CONNECTION):
                sb.appendString("Rua")
                    .append(1)
                    .append(151)
                    .append(151)
                    .appendString("Rua")
                    .appendString(uuid)
                    .append(2022)
                    .distrib(PacketType::REGISTER_CONNECTION);
                ch->write(&sb);
                break;
            default:
                sb.append<char>(0)
                    .append<int>(3)
                    .append<char>(0)
                    .append<int>(1)
                    .appendString("RELAYCN")
                    .distrib(PacketType::REDIRECT);
                ch->write(&sb);
            }
        };
        setThreadNum(10);
        setLoadBalance(LB_LeastConnections);
        start();
        Uplist();
	}

void Rua::Uplist()
{
    heads.clear();
    heads["User-Agent"] = "rw pc 151 zh";
    heads["Language"] = "zh";
    heads["Content-Type"] = "application/x-www-form-urlencoded";
    heads["Connection"] = "Keep-Alive";
    heads["Accept-Encoding"] = "identity";
    auto resp = requests::post("http://http.api.data.der.kim/UpList/v5/upList", "Version=HPS%231", heads);
    json apiRecv;
    if (resp == NULL) {
        printf("request failed!\n");
    }
    else {
        //printf("%s\n", resp->body.c_str());
        apiRecv = json::parse(resp->body);
        auto addInfo = apiRecv["add"].get<string>();
        auto id = apiRecv["id"].get<string>();
        auto decodeduuid = Base64Decode(id.c_str(), id.size());
        strcpy(uuid, decodeduuid.c_str());
        auto decodedAddInfo = Base64Decode(addInfo.c_str(), addInfo.size());
        //printf("%s\n", decodedAddInfo.c_str());
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.RW.VERSION.INT}", "151");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.RW.VERSION}", "1.14");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.RW.IS.VERSION}", "false");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.RW.IS.PASSWD}", "false");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.S.NAME}", "RuaTest");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.S.PRIVATE.IP}", "10.0.0.1");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.S.PORT}", "5555");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.RW.MAP.NAME}", "\001\001Unofficial+CN+Link");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.PLAYER.SIZE}", "1");
        Utils::matchAndRepl(decodedAddInfo, "{RW-HPS.PLAYER.SIZE.MAX}", "1000");
        //printf("%s\n", decodedAddInfo.c_str());
        auto resp2 = requests::post("http://gs1.corrodinggames.com/masterserver/1.4/interface", decodedAddInfo, heads);
        if (resp2 == NULL)
        {
            printf("UP failed\n");
        }
        else
        {
            printf("UP succeed\n");
        }
        auto openInfo = apiRecv["open"].get<string>();
        auto decodedOpenInfo = Base64Decode(openInfo.c_str(), openInfo.size());
        Utils::matchAndRepl(decodedOpenInfo, "{RW-HPS.S.PORT}", "5555");
        auto resp3 = requests::post("http://gs1.corrodinggames.com/masterserver/1.4/interface", decodedOpenInfo, heads);
        if (resp3 == NULL)
        {
            printf("UP failed\n");
        }
        else
        {
            printf("open succeed\n");
        }
        //printf("open: %s\n", decodedOpenInfo.c_str());
        auto udInfo = apiRecv["update"].get<string>();
        auto decodedudInfo = Base64Decode(udInfo.c_str(), udInfo.size());
        
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.RW.IS.PASSWD}", "false");
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.S.NAME}", "RuaTest");
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.S.PRIVATE.IP}", "10.0.0.1");
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.S.PORT}", "5555");
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.RW.MAP.NAME}", "\002\002Unofficial+CN+Link");
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.PLAYER.SIZE}", "1");
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.PLAYER.SIZE.MAX}", "1000");
        Utils::matchAndRepl(decodedudInfo, "{RW-HPS.S.STATUS}", "battleroom");
        //printf("%s\n", decodedudInfo.c_str());
        UpdateString = decodedudInfo;
    }
}

void Rua::UpdateList()
{
    auto resp = requests::post("http://gs1.corrodinggames.com/masterserver/1.4/interface", UpdateString, heads);
    if (resp == NULL)
    {
        printf("Update Failed\n");
    }
    else
    {
        printf("Update Succeed\n");
    }
}

