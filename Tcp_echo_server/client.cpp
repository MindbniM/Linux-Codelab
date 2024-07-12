#include<iostream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include"log.hpp"
int main()
{
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0) 
    {
        FATAL("socket error\n");
        exit(1);
    }
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(8848);
    server.sin_addr.s_addr=inet_addr("47.121.132.64");
    int n=connect(fd,(struct sockaddr*)&server,sizeof(server));
    if(n<0) 
    {
        FATAL("link error\n");
        exit(1);
    }
    while(1)
    {
        std::string str;
        std::getline(std::cin,str);
        if(str=="quit") break;
        int n=send(fd,str.c_str(),str.size(),0);
        if(n>0)
        {
            char buff[1024]={0};
            n=recv(fd,buff,sizeof(buff)-1,0);
            if(n>0)
            {
                std::cout<<buff<<std::endl;
            }

        }

    }
    return 0;
}