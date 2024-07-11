#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <string>
#include <ctime>
namespace log
{
#define FILEOUT 0x1
#define STDOUT 0x2
    class log
    {
    public:
        log(const log &l) = delete;
        log &operator=(const log l) = delete;
        void out(const std::string &file, int line, const char *str, ...)
        {
            char buff[1024] = {0};
            va_list va;
            va_start(va, str);
            vsnprintf(buff, 1023, str, va);
            va_end(va);
            std::string log = "[" + get_time() + "][" + _log_flag + "][file:" + file + "][line:" + std::to_string(line) + "]:" + buff;
            if (_out_flag & FILEOUT )
            {
                if(!_file.is_open())
                {
                    _file.open(_fname);
                }
                _file << log;
            }
            else if (_out_flag & STDOUT)
            {
                std::cout << log;
            }
        }
        static void file_out()
        {
            debug->_out_flag=FILEOUT;
            info->_out_flag=FILEOUT;
            warning->_out_flag=FILEOUT;
            error->_out_flag=FILEOUT;
            fatal->_out_flag=FILEOUT;
        }
        static log *get_debug()
        {
            return debug;
        }
        static log *get_info()
        {
            return info;
        }
        static log *get_warning()
        {
            return warning;
        }
        static log *get_error()
        {
            return error;
        }
        static log *get_fatal()
        {
            return fatal;
        }
        ~log()
        {
            if (_out_flag & FILEOUT && _file.is_open())
            {
                _file.close();
            }
        }
        static log *get_static_debug()
        {
            return debug;
        }

    private:
        log(const std::string &file_name, int flag) : _fname(file_name), _out_flag(flag), _log_flag(file_name)
        {
            if (flag & FILEOUT)
            {
                _file.open(_fname, std::ios::app);
                if (!_file.is_open())
                {
                    std::cout << "file open error" << std::endl;
                }
            }
        }
        std::string get_time()
        {
            const time_t t = time(nullptr);
            struct tm *pt = localtime(&t);
            char buff[64];
            snprintf(buff, 64, "%d-%d-%d %d:%d", pt->tm_year + 1900, pt->tm_mon + 1, pt->tm_mday, pt->tm_hour, pt->tm_min);
            return buff;
        }
        std::string _fname;
        int _out_flag;
        std::string _log_flag;
        std::fstream _file;
        static log *debug;
        static log *info;
        static log *warning;
        static log *error;
        static log *fatal;
    };
    log *log::debug = new log("debug", STDOUT);
    log *log::info = new log("info", STDOUT);
    log *log::warning = new log("warning", STDOUT);
    log *log::error = new log("error", STDOUT);
    log *log::fatal = new log("fatal", STDOUT);
#define DEBUG(str, ...)                                                    \
    do                                                                     \
    {                                                                      \
        log::log::get_debug()->out(__FILE__, __LINE__, str, ##__VA_ARGS__); \
    } while (0)
#define INFO(str, ...)                                                    \
    do                                                                    \
    {                                                                     \
        log::log::get_info()->out(__FILE__, __LINE__, str, ##__VA_ARGS__); \
    } while (0)
#define WARNING(str, ...)                                                    \
    do                                                                       \
    {                                                                        \
        log::log::get_warning()->out(__FILE__, __LINE__, str, ##__VA_ARGS__); \
    } while (0)
#define ERROR(str, ...)                                                    \
    do                                                                     \
    {                                                                      \
        log::log::get_error()->out(__FILE__, __LINE__, str, ##__VA_ARGS__); \
    } while (0)
#define FATAL(str, ...)                                                    \
    do                                                                     \
    {                                                                      \
        log::log::get_fatal()->out(__FILE__, __LINE__, str, ##__VA_ARGS__); \
    } while (0)
}