#pragma once
#include"http_service.hpp"
class handle
{
public:
    void static _recv(Connect *con)
    {
        errno = 0;
        while (1)
        {
            char buff[1024] = {0};
            int n = recv(con->sockfd(), buff, 1023, 0);
            if (n > 0)
            {
                buff[n] = 0;
                con->inbuff() += buff;
                // 交付给上层协议处理报文
                con->outbuff()+="#:"+std::string(buff);
                //应答
                con->send(con);
            }
            else
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    LOG(INFO, "get all data");
                    return;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    LOG(WARNING, "recv error");
                    break;
                }
            }
        }
        con->excepter(con);
    }
    void static _send(Connect *con)
    {
        errno = 0;
        while (1)
        {
            int n = send(con->sockfd(), con->outbuff().c_str(), con->outbuff().size(), 0);
            if (n > 0)
            {
                con->outbuff().erase(0, n);
                if (con->outbuff().empty())
                {
                    if (con->events() & EPOLLOUT)
                    {
                        con->self_in()->updata_r(con->sockfd());
                    }
                    return;
                }
            }
            else
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    LOG(INFO, "对方缓冲区已写满");
                    con->self_in()->updata_rw(con->sockfd());
                    return;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    LOG(WARNING, "send error");
                    break;
                }
            }
        }
        con->excepter(con);
    }
    void static _excepter(Connect *con)
    {
        LOG(INFO, "link quie fd:%d ip:%s", con->sockfd(), con->addr().c_str());
        con->self_in()->del(con->sockfd());
    }
};