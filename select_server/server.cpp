#include"socket.hpp"
#include<sys/types.h>
#include<sys/select.h>
#include<algorithm>
#include<set>
#include<vector>
const int N=1024;
class select_server : public Socket::Tcp_server
{
public:
    select_server()
    {
        Tcp_server_init(8848);
        _set.insert(_listenfd);
    }
    void comment_insert()
    {
        struct sockaddr_in in;
        socklen_t len=sizeof(in);
        int fd=accept(_listenfd,(struct sockaddr*)&in,&len);
        inaddr poor(in.sin_addr.s_addr,in.sin_port);
        _set.insert(fd);
        INFO("get new comment: %s:%s\n",poor.s_ip().c_str(),poor.s_prot().c_str());
    }
    void disposs(fd_set* arr)
    {
        for(int i=0;i<N;i++)
        {
            if(FD_ISSET(i,arr))
            {
                if(i==_listenfd)
                {
                    comment_insert();
                }
                else 
                {
                    char buff[1024]={0};
                    int n=recv(i,buff,N,0);
                    if(n>0)
                    {
                        INFO("get message :%s",buff);
                        std::string str="#"+std::string(buff);
                        send(i,str.c_str(),str.size(),0);
                    }
                    else if(n==0)
                    {
                        INFO("link closs %d\n",i);
                        _set.erase(i);
                    }
                }
            }
        }
    }
    void start()
    {
        while(1)
        {
            fd_set arr;
            FD_ZERO(&arr);
            FD_SET(socket_fd(),&arr);
            for(auto& i:_set)
            {
                FD_SET(i,&arr);
                std::cout<<i<<" ";
            }
            std::cout<<std::endl;
            int maxfd=*_set.rbegin();
            int n=select(maxfd+1,&arr,nullptr,nullptr,nullptr);
            if(n>0)
            {
                disposs(&arr);
            }
            sleep(1);
        }
    }
private:
    std::set<int> _set;
};
int main()
{
    select_server server;
    server.start();
    return 0;
}