#include"common.h"
#include"Rua.h"
#pragma comment(lib, "hv_static.lib")
int main()
{
	Rua r1(5555);
    EventLoopPtr loop(new EventLoop);
    loop->setInterval(60000, [&](TimerID timerID) {
        r1.UpdateList();
        });
    loop->run();
	while (getchar() != '\n');
}