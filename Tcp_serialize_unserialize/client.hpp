#include "socket.hpp"
#include"protocol.hpp"
#include<unistd.h>
class Factory
    {
    public:
        Factory()
        {
            srand(time(nullptr));
            opers = "+/*/%^&|";
        }
        protocol_ns::request BuildRequest()
        {
            int x = rand() % 10 + 1;
            usleep(x * 10);
            int y = rand() % 5; // [01,2,3,4]
            usleep(y * x * 5);
            char oper = opers[rand() % opers.size()];
            protocol_ns::request req(x, y, oper);
            return req;
        }
        ~Factory()
        {
        }

    private:
        std::string opers;
    };
class client : public Socket::Tcp_client
{
public:
    client()
    {}
    bool init(inaddr &server)
    {
        return Tcp_client_init(server);
    }
    template<class T1=typename::Socket::_default_,class T2=typename::Socket::_default_>
    void start(T1 rec=typename::Socket::_default_(),T2 sen=typename::Socket::_default_())
    {
        Factory f;
        while (1)
        {
            sleep(1);
            protocol_ns::request re=f.BuildRequest();
            std::string str=re.serialize();
            int n = Send(str,sen);
            if (n > 0)
            {
                std::string re;
                n = Recv(re,rec);
                if (n > 0)
                {
                    std::cout << re << std::endl;
                }
            }
        }
    }

private:
};
