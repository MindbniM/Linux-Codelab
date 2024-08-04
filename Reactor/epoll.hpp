#pragma once
#include<iostream>
#include<sys/epoll.h>
#include"excepter.hpp"
#include"log.hpp"
template<size_t N=64>
class epoll
{
public:
    epoll():_fd(-1)
    {
        _fd=epoll_create(1);
        if(_fd<0)
        {
            LOG(FATAL,"epoll_create error");
            exit(EPOLL_CREATE_ERROR);
        }
        LOG(INFO,"epoll_create success fd:%d",_fd);
    }
    void insert(int fd,const std::string& events)
    {
        ctl(fd,EPOLL_CTL_ADD,events,"add ");
    }
    void updata(int fd,const std::string& events)
    {
        ctl(fd,EPOLL_CTL_MOD,events,"updata ");
    }
    void del(int fd)
    {
        ctl(fd,EPOLL_CTL_DEL,"","del ");
    }
    void ctl(int fd,int flag,const std::string& events,const std::string& log)
    {
        uint32_t e=EPOLLET;
        for(auto& c:events)
        {
            if(c=='r') e|=EPOLLIN;
            else if(c=='w') e|=EPOLLOUT;
        }
        struct epoll_event ev;
        ev.events=e;
        ev.data.fd=fd;
        struct epoll_event* p=&ev;
        if(events=="") p=nullptr;
        int n=epoll_ctl(_fd,flag,fd,p);
        std::string str="epoll_ctl "+log;
        if(n<0)
        {
            str+="error";
            LOG(ERROR,str.c_str());
        }
        str+="success fd:%d"+std::string(" ")+events;
        LOG(INFO,str.c_str(),fd);
    }
    int wait(int timeout=0)
    {
        return epoll_wait(_fd,_arr,N,timeout);
    }
    struct epoll_event* event()
    {
        return _arr;
    }
    ~epoll()
    {
        if(_fd>0)
        {
            close(_fd);
        }
    }
private:
    int _fd;
    struct epoll_event _arr[N];
};