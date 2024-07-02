#include<cstring>
#include<arpa/inet.h>
#include<strings.h>
#include<netinet/in.h>
#include<cerrno>
#include<cstdlib>
#include<sys/types.h>
#include<sys/socket.h>
#include"log.hpp"
class echo_server
{
public:
    echo_server():_socketfd(-1),_port(8848)
    {}
    void server_init()
    {
        _socketfd=socket(AF_INET,SOCK_DGRAM,0);
        if(_socketfd==-1)
        {
            FATAL("socket error:%s\n",strerror(errno));
            exit(-1);
        }
        INFO("socket create success fd:%d\n",_socketfd);
        struct sockaddr_in s;
        bzero(&s,sizeof(s));
        s.sin_family=AF_INET;
        s.sin_addr.s_addr=INADDR_ANY;
        s.sin_port=htons(_port);
        int n=bind(_socketfd,(struct sockaddr*)&s,sizeof(s));
        if(n==-1)
        {
            FATAL("bind error:%s\n",strerror(errno));
            exit(-1);
        }
        INFO("bind success\n");
    }
    void server_start()
    {
        for(;;)
        {
            struct sockaddr_in temp;
            socklen_t len=sizeof(temp);
            memset(&temp,0,sizeof(temp));
            char buff[1024];
            ssize_t n=recvfrom(_socketfd,buff,sizeof(buff)-1,0,(struct sockaddr*)&temp,&len);
            if(n==-1)
            {
                ERROR("recvfrom error:%s",strerror(errno));
            }
            else 
            {
                buff[n]='\0';
                INFO("ip:%s:%d  %s",inet_ntoa(temp.sin_addr),ntohs(temp.sin_port),buff);
            }
        }
    }
private:
    int _socketfd;
    int _port; 
};