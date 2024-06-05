#pragma once
#include<functional>
template<class T>
class Thread
{
public:
    Thread()
    {}
    Thread(const std::function<void(T)>& func,T args,const std::string& name):_thread_name(name),_func(func),_args(args)
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
    T _args;
    pthread_t _tid;
    std::function<void(T)> _func;
    static void* _thread(void*args)
    {
        Thread<T>* p=(Thread<T>*)args;
        p->_func(p->_args);
        return nullptr;
    }
};