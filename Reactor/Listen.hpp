#pragma once
#include "Reactor.hpp"
#include "inaddr.hpp"
#include "log.hpp"
#include "excepter.hpp"
#include"handle.hpp"
class Listen
{
public:
    Listen(uint16_t prot):_server(INADDR_ANY,htons(prot))
    {
        _listenfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenfd < 0)
        {
            LOG(FATAL, "socket create error");
            exit(SOCKET_ERROR);
        }
        LOG(INFO, "socket create success fd:%d", _listenfd);
        int opt=1;
        setsockopt(_listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        struct sockaddr_in in;
        memset(&in, 0, sizeof(in));
        in.sin_family = AF_INET;
        in.sin_addr.s_addr = INADDR_ANY;
        in.sin_port = htons(prot);
        int n = bind(_listenfd, (struct sockaddr *)&in, sizeof(in));
        if (n < 0)
        {
            LOG(FATAL, "bind error");
            exit(BIND_ERROR);
        }
        LOG(INFO, "bind success");
        n = ::listen(_listenfd, 8);
        if (n < 0)
        {
            LOG(FATAL, "listen error");
            exit(LISTEN_ERROR);
        }
        LOG(INFO, "listen success");
    }
    //获取连接
    void accept(Connect *con)
    {
        while (1)
        {
            struct sockaddr_in poor;
            socklen_t len = sizeof(poor);
            memset(&poor, 0, len);
            int fd = ::accept(_listenfd, (struct sockaddr *)&poor, &len);
            inaddr in(poor.sin_addr.s_addr, poor.sin_port);
            if (fd > 0)
            {
                LOG(INFO, "get new link fd:%d", fd);
                con->self_in()->insert(fd, "r", in, handle::_recv, handle::_send,handle:: _excepter);
            }
            else
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    LOG(INFO, "get all link");
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    LOG(WARNING, "accept error");
                    break;
                }
            }
        }
    }
    int fd()
    {
        return _listenfd;
    }
    inaddr addr()
    {
        return _server;
    }
    ~Listen()
    {
        if(_listenfd>0)
        {
            close(_listenfd);
        }
    }

private:
    int _listenfd;
    inaddr _server;
};