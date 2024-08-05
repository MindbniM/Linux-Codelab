#pragma once
#include<map>
#include"epoll.hpp"
#include"connect.hpp"
#include<fcntl.h>
class Reactor
{
public:
    void insert(int fd,const std::string& event,inaddr poor,func_t recv,func_t send,func_t exc)
    {
        if(fd<0) return ;
        int fl=fcntl(fd,F_GETFL);
        fcntl(fd,F_SETFL,fl|O_NONBLOCK);
        Connect* c=new Connect(fd,this,event,poor);
        c->set_func(recv,send,exc);
        _connects[fd]=c;
        _e.insert(fd,event);
    }
    bool count(int fd)
    {
        return _connects.count(fd);
    }
    void updata_rw(int fd)
    {
        _e.updata(fd,"rw");
    }
    void updata_r(int fd)
    {
        _e.updata(fd,"r");
    }
    void del(int fd)
    {
        if(!_connects.count(fd)) return ;
        _e.del(fd);
        _connects[fd]->close();
        delete _connects[fd];
        _connects.erase(fd);
    }
    void looponce(int timeout)
    {
        int n=_e.wait(timeout);
        struct epoll_event* arr=_e.event();
        //debug
        std::string log;
        for(int i=0;i<n;i++)
        {
            log+=std::to_string(arr[i].data.fd)+" ";
            uint32_t ev=arr[i].events;
            if(ev&EPOLLIN)
            {
                log+="r\n";
            }
            else if(ev&EPOLLOUT)
            {
                log+="w/n";
            }
        }
        if(n!=0)
        {
            log+="is OK";
            LOG(INFO,log.c_str());
        }
        //debug
        for(int i=0;i<n;i++)
        {
            int fd=arr[i].data.fd;
            uint32_t ev=arr[i].events;
            if (ev & EPOLLHUP)
                ev |= (EPOLLIN | EPOLLOUT);
            if (ev & EPOLLERR)
                ev |= (EPOLLIN | EPOLLOUT);
            if((ev&EPOLLIN)&&count(fd))
            {
                LOG(INFO,"fd:%d running...recv",fd);
                _connects[fd]->recv(_connects[fd]);
            }
            if((ev&EPOLLOUT)&&count(fd))
            {
                LOG(INFO,"fd:%d running...send",fd);
                _connects[fd]->send(_connects[fd]);
            }
        }
    }
    void Dispatcher()
    {
        int timeout=3000;
        while(1)
        {
            looponce(timeout);
        }
    }
private:
    std::map<int,Connect*> _connects;
    epoll<> _e;
};