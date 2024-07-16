#include "socket.hpp"
#include "protocol.hpp"
#include <thread>

class server : public Socket::Tcp_server
{
public:
    server()
    {
    }
    void init(uint16_t prot)
    {
        Tcp_server_init(prot);
    }
    template<class T1=typename::Socket::_default_,class T2=typename::Socket::_default_>
    void start(T1 re=typename::Socket::_default_(),T2 se=typename::Socket::_default_())
    {
        while (1)
        {

            inaddr peer;
            Tcp_client_ptr p = socket_accept(peer);
            if (p == nullptr)
                continue;
            std::string str = "[";
            str += peer.s_ip();
            str += ":" + peer.s_prot() + "]:";
            INFO("%s link success  fd:%d\n", str.c_str(), p->socket_fd());
            std::thread t(&server::_start<T1,T2>, this, p,re,se);
            t.detach();
        }
    }
    template<class T1=typename::Socket::_default_,class T2=typename::Socket::_default_>
    void _start(Tcp_client_ptr p,T1 re=typename::Socket::_default_(),T2 se=typename::Socket::_default_())
    {
        while (1)
        {
            sleep(1);
            protocol_ns::request r;
            std::string str;
            int n = Recv(p, str,re);
            if (n > 0)
            {
                protocol_ns::response rs;
                INFO("%s\n", str.c_str());
                r.unserialize(str);
                switch (r._char)
                {
                case '+':
                {
                    rs._ret = r._x + r._y;
                    break;
                }
                case '-':
                {
                    rs._ret = r._x - r._y;
                    break;
                }
                case '*':
                {
                    rs._ret = r._x * r._y;
                    break;
                }
                case '/':
                {
                    if (r._char == '/' && r._y == 0)
                    {
                        rs._ret = 0;
                        rs._code = 1;
                    }
                    else rs._ret=r._x/r._y;
                    break;
                }
                default:
                    rs._code=3;
                    break;
                }
                std::string rstr=rs.serialize();
                Send(p, rstr,se);
            }
            else if (n == 0)
            {
                INFO("quit\n");
                break;
            }
        }
    }

private:
    inaddr self;
};