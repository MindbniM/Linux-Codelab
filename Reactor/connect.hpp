#pragma once
#include "inaddr.hpp"
#include <functional>
class Reactor;
class Connect;
using func_t = std::function<void(Connect *)>;
class Connect
{
public:
    Connect(int fd, Reactor *root,const std::string& event,inaddr in) : _fd(fd), _root(root),_inaddr(in)
    {
        //默epoll以ET模式
        uint32_t e=EPOLLET;
        for(auto& c:event)
        {
            if(c=='r') e|=EPOLLIN;
            else if(c=='w') e|=EPOLLOUT;
        }
        _event.data.fd=fd;
        _event.events=e;
    }
    int sockfd()
    {
        return _fd;
    }
    void set_func(func_t recv, func_t send, func_t excepter)
    {
        _recv=recv;
        _send=send;
        _excepter=excepter;
    }
    void set_event(uint32_t event)
    {
        _event.events=event;
    }
    void recv(Connect* con)
    {
        if(_recv!=nullptr)
        _recv(con);
    }
    void send(Connect* con)
    {
        if(_send!=nullptr)
        _send(con);
    }
    void excepter(Connect*con)
    {
        if(_excepter!=nullptr)
        _excepter(con);
    }
    Reactor* self_in()
    {
        return _root;
    }
    void setip(inaddr poor)
    {
        _inaddr=poor;
    }
    std::string addr()
    {
        return _inaddr.s_ip()+" "+_inaddr.s_prot();
    }
    std::string& inbuff()
    {
        return _in;
    }
    std::string& outbuff()
    {
        return _out;
    }
    void close()
    {
        if(_fd>0)
        ::close(_fd);
    }
    uint32_t events()
    {
        return _event.events;
    }
private:
    int _fd;
    std::string _in;
    std::string _out;

    struct epoll_event _event;
    inaddr _inaddr;
    func_t _recv;
    func_t _send;
    func_t _excepter;
    Reactor *_root;
};