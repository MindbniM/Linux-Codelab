#include<iostream>
#include<set>
#include<cstring>
#include"log.hpp"
#include"sockaddrin.hpp"
#include"user.hpp"
#define MAX_SIZE 1024
enum 
{
    SOCKET_ERROR=1,
    BIND_ERROR,
};

class server
{
public:
    server():_addr(INADDR_ANY,htons(8848))
    {}
    void init()
    {
        _sockfd=socket(AF_INET,SOCK_DGRAM,0);
        if(_sockfd<0)
        {
            FATAL("socket error\n");
            exit(SOCKET_ERROR);
        }
        INFO("socketfd success\n");
        struct sockaddr_in temp;
        memset(&temp,0,sizeof(temp));
        temp.sin_family=AF_INET;
        temp.sin_addr.s_addr=_addr.inet_ip();
        temp.sin_port=_addr.inet_prot();
        int n=bind(_sockfd,(sockaddr*)&temp,sizeof(temp));
        if(n<0)
        {
            FATAL("bind error\n");
            exit(BIND_ERROR);
        }
        INFO("bind success\n");

    }
    void start()
    {
        while(1)
        {
            char buff[MAX_SIZE]={0};
            struct sockaddr_in peer;
            memset(&peer,0,sizeof(peer));
            socklen_t len=sizeof(peer);
            int n=recvfrom(_sockfd,buff,MAX_SIZE,0,(struct sockaddr*)&peer,&len);
            if(n>0)
            {
                sockaddrin temp(peer.sin_addr.s_addr,peer.sin_port);
                user u(temp.inet_ip(),temp.inet_prot());
                INFO("recv...to...%s",u._name.c_str());
                if(strcmp(buff,"join in")==0)
                {
                    INFO("add user\n");
                    _u.add_user(u);
                    //...添加
                }
                put_message(buff,u);
                if(strcmp(buff,"quit")==0)
                {
                    INFO("quit user\n");
                    _u.quit_user(u);
                    //...删除退出
                }
            }
        }
    }
    void put_message(const std::string& str,user pu)
    {
        users::iterator it=_u.begin();
        while(it!=_u.end())
        {
            user u=*it;
            struct sockaddr_in peer;
            memset(&peer,0,sizeof(peer));
            peer.sin_family=AF_INET;
            peer.sin_addr.s_addr=u._ip;
            peer.sin_port=u._prot;
            std::string message="\n["+pu._name+"]:"+str+"\n";
            sendto(_sockfd,message.c_str(),message.size(),0,(struct sockaddr*)&peer,sizeof(peer));
            ++it;
        }
    }
private:
    int _sockfd=-1;
    sockaddrin _addr;
    users _u;
};