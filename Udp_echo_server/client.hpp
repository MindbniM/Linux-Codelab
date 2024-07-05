#include"log.hpp"
#include<iostream>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
class client
{
public:
    client()
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
        memset(&_host,0,sizeof(_host));
        _host.sin_port=htons(_prot);
        _host.sin_family=AF_INET;
        _host.sin_addr.s_addr=inet_addr("47.121.132.64");
    }
    void strat()
    {
        is_run=true;
        while(1)
        {
            std::string str;
            std::cout<<"get string"<<std::endl;
            std::getline(std::cin,str);
            ssize_t n=sendto(_socket_fd,str.c_str(),str.size(),0,(struct sockaddr*)&_host,sizeof(_host));
            if(n>0)
            {
                INFO("to server......\n");
            }
            
        }
    }
private:
    int _socket_fd;
    uint16_t _prot=8848;
    bool is_run=false;
    sockaddr_in _host;
};