#pragma once
#include<functional>
class Thread
{
public:
    Thread()
    {}
    Thread(const std::function<void()>& func,const std::string& name):_thread_name(name),_func(func)
    {}
    bool start()
    {
        int n=pthread_create(&_tid,nullptr,_thread,(void*)this);
        if(n!=0) return false;
        return true;
    }
    void join()
    {
        pthread_join(_tid,nullptr);
    }
    std::string get_thread_name()
    {
        return _thread_name;
    }
    pthread_t gettid()
    {
        return _tid;
    }
private:
    std::string _thread_name;
    pthread_t _tid;
    std::function<void()> _func;
    static void* _thread(void*args)
    {
        Thread* p=(Thread*)args;
        p->_func();
        return nullptr;
    }
};