#pragma once
#include<cstdio>
#include <iostream>
#include <fstream>
#include<cstdarg>
#include <string>
#include<ctime>
namespace log
{
#define FILEOUT 0x1
#define STDOUT 0x2
    class log
    {
    public:            
        log(const log& l)=delete;
        log& operator=(const log l)=delete;
        void out(const std::string& file,int line,const char* str,...)
        {
            char buff[1024]={0};
            va_list va;
            va_start(va,str);
            vsnprintf(buff,1023,str,va);
            va_end(va);
            std::string log="["+get_time()+"]["+_log_flag+"][file:"+file+"][line:"+std::to_string(line)+"]["+buff+"]";
            if(_out_flag& FILEOUT&&_file.is_open())
            {
                _file<<log<<std::endl;
            }
            else if(_out_flag&STDOUT) 
            {
                std::cout<<log<<std::endl;
            }
        }
        log(const std::string &file_name, int flag) : _fname(file_name), _out_flag(flag)
        {
            if (flag & FILEOUT)
            {
                _file.open(_fname,std::ios::app);
                if(!_file.is_open())
                {
                    std::cout<<"file open error"<<std::endl;
                }
            }
        }
        std::string get_time() 
        {
            const time_t t=time(nullptr);
            struct tm* pt=localtime(&t);
            char buff[64];
            snprintf(buff,64,"%d-%d-%d %d:%d",pt->tm_year+1900,pt->tm_mon+1,pt->tm_mday,pt->tm_hour,pt->tm_min);
            return buff;
        }
        ~log()
        {
            if (_out_flag & FILEOUT && _file.is_open())
            {
                _file.close();
            }
        }
        
        std::string _fname;
        int _out_flag;
        std::string _log_flag;
        std::fstream _file;
        static log log_debug;
        static log log_info;
        static log log_warning;
        static log log_error;
        static log log_fatal;
    };
}