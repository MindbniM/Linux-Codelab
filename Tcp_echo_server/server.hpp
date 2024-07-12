#include<iostream>
#include<unistd.h>
#include<cstring>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"log.hpp"
#include<thread>
class server
{
public:
    server():_listenfd(-1)
    {}
    void init()
    {
        _listenfd=socket(AF_INET,SOCK_STREAM,0);
        if(_listenfd<0)
        {
            FATAL("socket error\n");
            exit(1);
        }
        INFO("socket success\n");
        struct sockaddr_in s;
        s.sin_family=AF_INET;
        s.sin_addr.s_addr=INADDR_ANY;
        s.sin_port=htons(8848);
        int n=bind(_listenfd,(struct sockaddr*)&s,sizeof(s));
        if(n<0)
        {
            FATAL("bind error\n");
            exit(2);
        }
        INFO("bind success\n");
        n=listen(_listenfd,16);
        if(n<0)
        {
            FATAL("listen error\n");
            exit(3);
        }
        INFO("listen success\n");
    }
    void start()
    {
        while(1)
        {
            struct sockaddr_in peer;
            memset(&peer,0,sizeof(peer));
            socklen_t len=sizeof(peer);
            int fd=accept(_listenfd,(struct sockaddr*)&peer,&len);
            if(fd<0) continue;
            struct in_addr temp=peer.sin_addr;
            std::string str="[";
            str+=inet_ntoa(temp);
            str+=":"+std::to_string(ntohs(peer.sin_port))+"]:";
            INFO("%s link success\n",str.c_str());
            std::thread t(&server::_start,this,fd,str);
            t.detach();
        }
    }
    void _start(int fd,std::string str)
    {
        while(1)
        {
            std::string s=str;
            char buff[1024]={0};
            ssize_t n=recv(fd,buff,sizeof(buff)-1,0);
            if(n>0)
            {
                s+=buff;
                INFO("%s\n",s.c_str());
                send(fd,s.c_str(),s.size(),0);
            }
            if(n==0)
            {
                str+="link quit";
                INFO("%s\n",str.c_str());
                break;
            }
        }
        close(fd);
    }
    ~server()
    {
        close(_listenfd);
    }
private:
    int _listenfd;
};