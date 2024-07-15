#include<iostream>
#include<unistd.h>
#include"server.hpp"
int main()
{
    Tcp_server client;
    inaddr server("47.121.132.64",htons(8848));
    if(!client.Tcp_client_init(server)) exit(1);

    //int fd=socket(AF_INET,SOCK_STREAM,0);
    //if(fd<0) 
    //{
    //    FATAL("socket error\n");
    //    exit(1);
    //}
    //struct sockaddr_in server;
    //server.sin_family=AF_INET;
    //server.sin_port=htons(8848);
    //server.sin_addr.s_addr=inet_addr("47.121.132.64");
    //int n=connect(fd,(struct sockaddr*)&server,sizeof(server));
    //if(n<0) 
    //{
    //    FATAL("link error\n");
    //    exit(1);
    //}
    while(1)
    {
        std::string str;
        std::getline(std::cin,str);
        if(str=="quit") break;
        int n=client.socket_sent(str);
        if(n>0)
        {
            std::string re;
            n=client.socket_recv(re);
            if(n>0)
            {
                std::cout<<re<<std::endl;
            }

        }

    }
    return 0;
}