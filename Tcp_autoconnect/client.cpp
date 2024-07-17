#include"socket.hpp"
#include<signal.h>
enum class status
{
    NEW=1,
    CONNECTING,
    DECONNECTED,
    CONNECTED,
    CLOSEED,
};
class test_client : public Socket::Tcp_client
{
public:
    test_client(inaddr& server):_server(server),_s(status::NEW)
    {
        if(Tcp_client_init(server))
        {
            _s=status::CONNECTED;
            INFO("link success\n");
        }
        else 
        {
            _s=status::DECONNECTED;
            INFO("link error\n");
        }
    }
    void start()
    {
        while(1)
        {
            switch (_s)
            {
                case status::CONNECTED:
                _start();
                break;
                case status::DECONNECTED:
                re_connect();
                break;
                case status::CLOSEED:
                return ;
            }
        }
    }
    void _start()
    {
        while(1)
        {
            sleep(1);
            std::string str="hello server";
            int n=Send(str);
            if(n<=0)
            {
                _s=status::DECONNECTED;
                WARNING("断开连接\n");
                break;
            }
        }
    }
    void re_connect()
    {
        close(_fd);
        socket_create();
        const int n=10;
        const int m=1;
        for(int i=0;i<n;i++)
        {
            sleep(1);
            if(socket_connect(_server))
            {
                INFO("重连成功\n");
                _s=status::CONNECTED;
                return;
            }
            INFO("正在第%d次重连\n",i+1);
        }
        INFO("连接失败,请检查网络\n");
        _s=status::CLOSEED;
    }
    status _s;
    inaddr _server;
};
int main()
{
    signal(SIGPIPE, SIG_IGN);
    inaddr server("47.121.132.64",htons(8848));
    test_client c(server);
    c.start();
    return 0;
}