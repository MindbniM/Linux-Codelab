#pragma once
#include<iostream>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
class fifo
{
public:
    fifo(const std::string& path="fifo"):_path(path),_rfd(-1),_wfd(-1)
    {
    }
    void creat()
    {
        int n=mkfifo(_path.c_str(),0666);
        if(n<0)
        {
            std::cout<<"creatfifo error"<<std::endl;
            exit(1);
        }
    }
    void Ropen()
    {
        _rfd=open(_path.c_str(),O_RDONLY);
        if(_rfd<0) 
        {
            std::cout<<"Ropen error"<<std::endl;
            exit(1);
        }
    }
    void Wopen()
    {
        _wfd=open(_path.c_str(),O_WRONLY);
        if(_wfd<0) 
        {
            std::cout<<"Wopen error"<<std::endl;
            exit(1);
        }
    }
    int Read()
    {
        char buff[64];
        int n=read(_rfd,buff,sizeof(buff)-1);
        return n;
    }
    void Write(const std::string& str="1")
    {
        ssize_t n=write(_wfd,str.c_str(),str.size());
    }
private:
    std::string _path;
    int _rfd;
    int _wfd;
};