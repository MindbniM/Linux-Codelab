#include"log.hpp"
#include<iostream>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
class server
{
public:
    server(uint16_t prot=8848):_socket_fd(-1),_prot(prot)
    {}
    void init() 
    {
        _socket_fd=socket(AF_INET,SOCK_DGRAM,0);
        if(_socket_fd<0)
        {
            FATAL("socket error\n");
            exit(-1);
        }
        INFO("socket create success\n");
        struct sockaddr_in temp;
        memset(&temp,0,sizeof(temp));
        temp.sin_family=AF_INET;
        temp.sin_port=htons(_prot);
        temp.sin_addr.s_addr=INADDR_ANY;
        int n=bind(_socket_fd,(struct sockaddr*)&temp,sizeof(temp));
        if(n<0)
        {
            FATAL("bind error\n");
            exit(-1);
        }
        INFO("bind success\n");
    }
    void strat()
    {
        is_run=true;
        while(1)
        {
            int buff[1024]={0};
            struct sockaddr_in peer;
            memset(&peer,0,sizeof(peer));
            socklen_t len=sizeof(peer);
            ssize_t n=recvfrom(_socket_fd,buff,sizeof(buff)-1,0,(struct sockaddr*)&peer,&len);
            if(n>0)
            {
                INFO("get margess:%s\n",buff);
            }
            INFO("run......\n");
        }
    }
private:
    int _socket_fd;
    uint16_t _prot;
    bool is_run=false;
};