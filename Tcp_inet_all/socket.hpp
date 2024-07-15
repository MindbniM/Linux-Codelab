#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<memory>
#include"inaddr.hpp"
namespace Socket
{
    class Socket;
    using socket_sptr=std::shared_ptr<Socket>;
    class Socket
    {
    public:
        virtual void socket_create()=0;
        virtual void socket_bind(uint16_t prot)=0;
        virtual void socket_listen(int n) =0;
        virtual socket_sptr socket_accept(inaddr& in)=0;
        virtual bool socket_connect(inaddr& server)=0;
        virtual int socket_fd()=0;
        virtual int socket_recv( std::string& str)=0;
        virtual int socket_sent(const std::string& in)=0;




        void Tcp_server_init(uint16_t prot)
        {
            socket_create();
            socket_bind(prot);
            socket_listen(16);
        }
        bool Tcp_client_init(inaddr& server)
        {
            socket_create();
            return socket_connect(server);
        }
        void Udp_server()
        {}
        void Udp_client()
        {}
    };
}
using socket_sptr=std::shared_ptr<Socket::Socket>;