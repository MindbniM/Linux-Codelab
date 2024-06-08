#pragma once
#include<iostream>
#include <string>
class log
{
protected:
    std::string _fname;
    bool _flag;
private:
    log(const std::string& file_name):_fname(file_name)
    {}
};

class log_debug :public log
{
public:
    log_debug()
}