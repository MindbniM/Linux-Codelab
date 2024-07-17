#include"socket.hpp"
#include<thread>
class test_server : public Socket::Tcp_server
{
public:
    void start()
    {
        while(1)
        {
            inaddr in;
            Tcp_client_ptr p=socket_accept(in);
            if(p==nullptr)
            {
                WARNING("accept error\n");
                continue;
            }
            std::string str;
            std::thread t(&test_server::_start,this,p,str);
            t.detach();
        }
    }
    void _start(Tcp_client_ptr p,std::string str)
    {
        int n=5;
        while(n)
        {
            sleep(1);
            Recv(p,str);
            INFO("%s",str.c_str());
            n--;
        }
    }
};
int main()
{
    test_server s;
    s.Tcp_server_init(8848);
    s.start();
    return 0;
}