#include<iostream>
#include<cerrno>
#include<cstring>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string>
#include"log.hpp"
int main()
{
    int fd=socket(AF_INET,SOCK_DGRAM,0);
    if(fd==-1)
    {
        FATAL("socket error:%s",strerror(errno));
        exit(-1);
    }
    INFO("socket create success\n");
    struct sockaddr_in temp;
    memset(&temp,0,sizeof(temp));
    temp.sin_addr.s_addr=inet_addr("127.0.0.1");
    temp.sin_family=AF_INET;
    temp.sin_port=htons(8848);

    for(;;)
    {
        std::string str;
        std::cout<<"put message"<<std::endl;
        std::getline(std::cin,str);
        ssize_t n=sendto(fd,str.c_str(),str.size(),0,(struct sockaddr*)&temp,sizeof(temp));
        if(n==-1)
        {
            ERROR("sendto error:%s",strerror(errno));
            exit(-1);
        }
        INFO("client to server message:%d\n",n);
    }
    return 0;
}