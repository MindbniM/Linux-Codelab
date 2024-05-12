#pragma once
#include <iostream>
#include <unistd.h>
#include <vector>
#include <functional>
#include<sys/wait.h>
#include<sys/types.h>
enum
{
    P_number_error = 1,
    Pipe_error,
};
template <class T>
class work
{
public:
    work(const std::string &name, std::function<T> work) : _name(name), _work(work)
    {
    }
    std::function<T> get()
    {
        return _work;
    }
private:
    std::string _name;
    std::function<T> _work;
};
template <class T>
class works
{
public:
    void Addwork( work<T> work)
    {
        _n++;
        _v.push_back(work);
    }
    int getNextworkid()
    {
        static int workid=0;
        if (_n != 0)
        {
            workid %= _n;
            return workid++;
        }
        return -1;
    }
    std::function<T> getwork(int id)
    {
        if (id < 0 || id >= _n)
            return nullptr;
        return _v[id].get();
    }

private:
    int _n = 0;
    std::vector<work<T>> _v;
};
class channel
{
public:
    channel(pid_t id, int fd, const std::string &name) : _pr_pid(id), _wfd(fd), _name(name)
    {
    }
    int getwfd()
    {
        return _wfd;
    }
    pid_t getpid()
    {
        return _pr_pid;
    }
private:
    pid_t _pr_pid;
    int _wfd;
    std::string _name;
};
class processpool
{
public:
    processpool(int n) : _n(n)
    {
    }
    template<class T>
    void CreateProcess(works<T>& wr)
    {
        for (int i = 0; i < _n; i++)
        {
            int fd[2];
            int num = pipe(fd);
            int rfd = fd[0], wfd = fd[1];
            if (num < 0)
                exit(Pipe_error);
            pid_t id = fork();
            if (id == 0)
            {
                close(wfd);
                for(auto& i:_v)
                {
                    close(i.getwfd());
                }
                dup2(rfd,0);
                work(wr);
                exit(0);
            }
            close(rfd);
            std::string str = "process_";
            _v.emplace_back(id, wfd, str += (char)(_n + '0'));
        }
    }
    int getNextprocess()
    {
        if(_n==0) return -1;
        static int c=0;
        c%=_n;
        return c++;
    }
    template<class T>
    void SendTaskCode(works<T>& wr)
    {
        int wfd=_v[getNextprocess()].getwfd();
        uint32_t wid=wr.getNextworkid();
        write(wfd,&wid,sizeof(wid));
    }
    ~processpool()
    {
        for(auto& i:_v)
        {
            close(i.getwfd());
            sleep(1);
            waitpid(i.getpid(),nullptr,0);
        }
    }
private:
    template <class T>
    void work(works<T> &wr)
    {
        while (true)
        {

            uint32_t command_id = 0;
            uint32_t n = read(0, &command_id, sizeof(command_id));
            if (n == sizeof(command_id))
            {
                auto w = wr.getwork(command_id);
                w();
                std::cout<<"pid: "<<getpid()<<"进程工作完成"<<std::endl;
            }
            else if(n==0)
            {
                exit(0);
            }
        }
    }
    int _n;
    std::vector<channel> _v;
};