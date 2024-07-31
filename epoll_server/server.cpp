#include"socket.hpp"
#include<sys/types.h>
#include<sys/select.h>
#include<sys/epoll.h>
template<size_t N=32>
class epoll_server : public Socket::Tcp_server
{
public:
    epoll_server():_epoll_fd(-1)
    {
        Tcp_server_init(8848);
        if(_epoll_fd=epoll_create(16);_epoll_fd<0)
        {
            FATAL("epoll create error\n");
            exit(1);
        }
        INFO("epoll create success\n");
        struct epoll_event ev;
        ev.events=EPOLLIN;
        ev.data.fd=_listenfd;
        epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,_listenfd,&ev);
    }
    void comment_insert()
    {
        struct sockaddr_in in;
        socklen_t len=sizeof(in);
        int fd=accept(_listenfd,(struct sockaddr*)&in,&len);
        struct epoll_event ev;
        ev.events=EPOLLIN;
        ev.data.fd=fd;
        epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,fd,&ev);
        inaddr poor(in.sin_addr.s_addr,in.sin_port);
        INFO("get new comment: %s:%s\n",poor.s_ip().c_str(),poor.s_prot().c_str());
    }
    void disposs(int n)
    {
        for(int i=0;i<n;i++)
        {
            int fd=_temp[i].data.fd;
            if(fd==_listenfd)
            {
                comment_insert();
            }
            else if(_temp[i].events&EPOLLIN)
            {
                char buff[1024]={0};
                int num=recv(fd,buff,sizeof(buff)-1,0);
                if(num>0)
                {
                    std::string str="#"+std::string(buff);
                    INFO("get message:%s",str.c_str());
                    send(fd,str.c_str(),str.size(),0);
                } 
                else if(num<=0)
                {
                    epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,fd,NULL);
                    close(fd);
                    INFO("%d:quit\n",fd);
                }
            }
        }
    }
    void start()
    {
        while(1)
        {
            int timeout=-1;
            int n=epoll_wait(_epoll_fd,_temp,N,-1);
            if(n>0)
            {
                disposs(n);
            }
        }
    }
    ~epoll_server()
    {
        close(_epoll_fd);
    }
private:
    int _epoll_fd;
    struct epoll_event _temp[N];
};
int main()
{
    epoll_server server;
    server.start();
    return 0;
}