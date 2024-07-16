#pragma once
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include "log.hpp"
#include<unistd.h>
#include "inaddr.hpp"
namespace Socket
{
    struct _default_
    {
        void operator()(std::string &str)
        {
        }
    };
    enum
    {
        SOCKET_ERROR = 1,
        BIND_ERROR,
        LISTEN_ERROR,
        CONNECT_ERROR,
    };

    class Socket;
    class Socket
    {
    public:
        virtual void socket_create() = 0;
        virtual int socket_fd() = 0;
    };

    class Tcp_client_Socket;
    class Tcp_client;
    using Tcp_client_ptr = std::shared_ptr<Tcp_client_Socket>;

    class Tcp_client_Socket : public Socket
    {
    public:
        virtual bool socket_connect(inaddr &server) = 0;
        virtual int socket_recv(std::string &str) = 0;
        virtual int socket_sent(const std::string &in) = 0;
        bool Tcp_client_init(inaddr &server)
        {
            socket_create();
            return socket_connect(server);
        }
    };

    class Tcp_client : public Tcp_client_Socket
    {
    public:
        Tcp_client(int fd = -1) : _fd(fd)
        {
        }
        template <class T=_default_>
        int Recv(std::string &str, T t = _default_())
        {
            int n=socket_recv(str);
            t(str);
            return n;
        }
        template <class T=_default_>
        int Send(std::string &str, T t = _default_())
        {
            str=t(str);
            int n=socket_sent(str);
            return n;
        }

    protected:
        int _fd;

        virtual bool socket_connect(inaddr &server) override
        {
            struct sockaddr_in ser;
            memset(&ser, 0, sizeof(ser));
            ser.sin_family = AF_INET;
            ser.sin_port = server.inet_prot();
            ser.sin_addr.s_addr = server.inet_ip();
            int n = connect(_fd, (struct sockaddr *)&ser, sizeof(ser));
            if (n < 0)
                return false;
            return true;
        }
        virtual int socket_recv(std::string &str) override
        {
            char buff[1024] = {0};
            int n = recv(_fd, buff, sizeof(buff) - 1, 0);
            if (n > 0)
            {
                str = std::string(buff);
            }
            return n;
        }
        virtual int socket_sent(const std::string &in) override
        {
            return send(_fd, in.c_str(), in.size(), 0);
        }
        virtual void socket_create() override
        {
            _fd = socket(AF_INET, SOCK_STREAM, 0);
            if (_fd < 0)
            {
                FATAL("socket error\n");
                exit(SOCKET_ERROR);
            }
            INFO("socket success\n");
        }
        virtual int socket_fd() override
        {
            return _fd;
        }
    };

    class Tcp_server_Socket : public Socket
    {
    public:
        virtual void socket_bind(uint16_t prot) = 0;
        virtual void socket_listen(int n) = 0;
        virtual Tcp_client_ptr socket_accept(inaddr &in) = 0;
        virtual int socket_recv(Tcp_client_ptr p, std::string &str) = 0;
        virtual int socket_sent(Tcp_client_ptr p, const std::string &in) = 0;
        void Tcp_server_init(uint16_t prot)
        {
            socket_create();
            socket_bind(htons(prot));
            socket_listen(16);
        }
    };

    class Tcp_server : public Tcp_server_Socket
    {
    public:
        Tcp_server() : _listenfd(-1)
        {
        }
        template<class T=_default_>
        int Recv(Tcp_client_ptr p, std::string &str, T t = _default_())
        {
            std::string s="";
            std::string restr;
            int n=1;
            while(s==""&&n!=0)
            {
                std::string peer;
                n=socket_recv(p, peer);
                restr+=peer;
                s=t(restr);
            }
            str=s;
            return s!="";
        }
        template <class T=_default_>
        int Send(Tcp_client_ptr p, std::string &str, T t = _default_())
        {
            t(str);
            int n=socket_sent(p, str);
            return n;
        }
        virtual ~Tcp_server()
        {
            close(_listenfd);
        }
    protected:
        int _listenfd;

        virtual int socket_recv(Tcp_client_ptr p, std::string &str) override
        {
            char buff[1024] = {0};
            int n = recv(p->socket_fd(), buff, sizeof(buff) - 1, 0);
            if (n > 0)
            {
                str = std::string(buff);
                std::cout<<str<<std::endl;
            }
            return n;
        }
        virtual int socket_sent(Tcp_client_ptr p, const std::string &in) override
        {
            return send(p->socket_fd(), in.c_str(), in.size(), 0);
        }
        virtual void socket_create() override
        {
            _listenfd = socket(AF_INET, SOCK_STREAM, 0);
            if (_listenfd < 0)
            {
                FATAL("socket error\n");
                exit(SOCKET_ERROR);
            }
            INFO("socket success\n");
        }
        virtual void socket_bind(uint16_t prot) override
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = prot;
            server.sin_addr.s_addr = INADDR_ANY;
            int n = bind(_listenfd, (struct sockaddr *)&server, sizeof(server));
            if (n < 0)
            {
                FATAL("bind error :%s\n ",strerror(errno));
                exit(BIND_ERROR);
            }
            INFO("bind success\n");
        }
        virtual void socket_listen(int n) override
        {
            int num = listen(_listenfd, n);
            if (num < 0)
            {
                FATAL("listen error\n");
                exit(LISTEN_ERROR);
            }
            INFO("listen success\n");
        }
        virtual int socket_fd() override
        {
            return _listenfd;
        }
        virtual Tcp_client_ptr socket_accept(inaddr &in) override
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            memset(&peer, 0, len);
            int fd = accept(_listenfd, (struct sockaddr *)&peer, &len);
            if (fd < 0)
            {
                WARNING("accept error\n");
                return nullptr;
            }
            in = inaddr(peer.sin_addr.s_addr, peer.sin_port);
            Tcp_client_ptr p = std::shared_ptr<Tcp_client_Socket>(new Tcp_client(fd));
            return p;
        }
    };

}
using Tcp_client_ptr = std::shared_ptr<Socket::Tcp_client_Socket>;