#include"http_server.hpp"
int main()
{
    http_server server(8848);
    server.start();
    
    return 0;
}