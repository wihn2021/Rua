#pragma once
#include "common.h"
#include "StreamBuilder.h"
#include<requests.h>
#include "Utils.h"

class Rua : public TcpServer
{
public:
    Rua(int);
    void Uplist();
    int port;
    void UpdateList();
private:
    char uuid[40];
    http_headers heads;
    string UpdateString;
};