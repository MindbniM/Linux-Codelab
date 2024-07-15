#include<iostream>
#include<unistd.h>
#include<cstring>
#include"socket.hpp"
#include"log.hpp"
#include<cerrno>
#include<thread>
enum
{
    SOCKET_ERROR=1,
    BIND_ERROR,
    LISTEN_ERROR
};
class Tcp_server :public Socket::Socket
{
public:
    Tcp_server(int prot,int fd=-1):_listenfd(fd),_ser("0",htons(prot))
    {}
    Tcp_server()
    {}
    void init()
    {
        Tcp_server_init(_ser.inet_prot());
    }

    //Tcp_server():_listenfd(-1)
    //{

    //}
    //void init()
    //{
    //    _listenfd=socket(AF_INET,SOCK_STREAM,0);
    //    if(_listenfd<0)
    //    {
    //        FATAL("socket error\n");
    //        exit(1);
    //    }
    //    INFO("socket success\n");
    //    struct sockaddr_in s;
    //    s.sin_family=AF_INET;
    //    s.sin_addr.s_addr=INADDR_ANY;
    //    s.sin_port=htons(8848);
    //    int n=bind(_listenfd,(struct sockaddr*)&s,sizeof(s));
    //    if(n<0)
    //    {
    //        FATAL("bind error\n");
    //        exit(2);
    //    }
    //    INFO("bind success\n");
    //    n=listen(_listenfd,16);
    //    if(n<0)
    //    {
    //        FATAL("listen error\n");
    //        exit(3);
    //    }
    //    INFO("listen success\n");
    //}
    void start()
    {
        while(1)
        {
            inaddr peer;
            socket_sptr p=socket_accept(peer);
            if(p==nullptr) continue;
            std::string str="[";
            str+=peer.s_ip();
            str+=":"+peer.s_prot()+"]:";
            INFO("%s link success  fd:%d\n",str.c_str(),p->socket_fd());
            std::thread t(&Tcp_server::_start,this,p,str);
            t.detach();
        }
    }
    void _start(socket_sptr p,std::string str)
    {
        while(1)
        {
            std::string s=str;
            std::string re;
            int n=p->socket_recv(re);
            if(n>0)
            {
                s+=re;
                INFO("%s\n",s.c_str());
                p->socket_sent(s);
            }
            else if(n==0)
            {
                str+="link quit";
                INFO("%s\n",str.c_str());
                break;
            }
            else 
            {
                std::cout<<"recv error:"<<strerror(errno)<<std::endl;
                exit(3);
            }
        }
    }
    virtual int socket_fd() override
    {
        return _listenfd;
    }
    virtual int  socket_recv(std::string& str) override
    {
        char buff[1024]={0};
        int n=recv(_listenfd,buff,sizeof(buff)-1,0);
        std::cout<<n<<std::endl;
        if(n>0)
        {
            str=std::string(buff);
        }
        return n;
    }
    virtual int socket_sent(const std::string& in) override
    {
        return send(_listenfd,in.c_str(),in.size(),0);
    }
    virtual socket_sptr socket_accept(inaddr& in)override
    {
        struct sockaddr_in peer;
        socklen_t len=sizeof(peer);
        memset(&peer,0,sizeof(peer));

        int n=accept(_listenfd,(struct sockaddr*)&peer,&len);
        if(n<0) return nullptr;
        struct in_addr temp=peer.sin_addr;
        in=inaddr(inet_ntoa(temp),peer.sin_port);
        socket_sptr p=std::make_shared<Tcp_server>(0,n);
        return p;
    }
    ~Tcp_server()
    {
        close(_listenfd);
    }
private:
    int _listenfd;
    inaddr _ser;
    


    virtual void socket_create() override
    {
        _listenfd=socket(AF_INET,SOCK_STREAM,0);
        if(_listenfd<0)
        {
            FATAL("socket error\n");
            exit(SOCKET_ERROR);
        }
        INFO("socket success\n");
    }
    virtual void socket_bind(uint16_t prot) override
    {
        struct sockaddr_in server;
        memset(&server,0,sizeof(server));
        server.sin_family=AF_INET;
        server.sin_port=prot;
        server.sin_addr.s_addr=INADDR_ANY;
        int n=bind(_listenfd,(struct sockaddr*)&server,sizeof(server));
        if(n<0)
        {
            FATAL("bind error\n");
            exit(BIND_ERROR);
        }
        INFO("bind success\n");
    }
    virtual void socket_listen(int n) override
    {
        int num=listen(_listenfd,n);
        if(num<0)
        {
            FATAL("listen error\n");
            exit(LISTEN_ERROR);
        }
        INFO("listen success\n");
    }
    
    virtual bool socket_connect(inaddr& server) override
    {
        struct sockaddr_in peer;
        memset(&peer,0,sizeof(peer));
        peer.sin_family=AF_INET;
        peer.sin_port=server.inet_prot();
        peer.sin_addr.s_addr=server.inet_ip();
        int n=connect(_listenfd,(struct sockaddr*)&peer,sizeof(peer));
        if(n<0) return false;
        return true;

    }
    
};