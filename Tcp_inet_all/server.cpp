#include"server.hpp"
int main()
{
    Tcp_server s(8848);
    s.init();
    s.start();

    return 0;
}