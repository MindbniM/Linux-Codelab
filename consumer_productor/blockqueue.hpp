#pragma once
#include<pthread.h>
#include<functional>
#include<queue>
using namespace std::placeholders;


template<class T>
class blockqueue
{
public:
    blockqueue(int cap=5):_cap(cap)
    {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_con_cond,nullptr);
        pthread_cond_init(&_pro_cond,nullptr);
    }
    void push(const T& data)
    {
        pthread_mutex_lock(&_mutex);
        while(full())
        {
            _con_wait_num++;
            pthread_cond_wait(&_con_cond,&_mutex);
            _con_wait_num--;
        }
        _q.push(data);
        if(_pro_wait_num>0) pthread_cond_broadcast(&_pro_cond);
        pthread_mutex_unlock(&_mutex);
    }
    T pop()
    {
        pthread_mutex_lock(&_mutex);
        while(empty())
        {
            _pro_wait_num++;
            pthread_cond_wait(&_pro_cond,&_mutex);
            _pro_wait_num--;
        }
        T temp=_q.front();
        _q.pop();
        if(_con_wait_num>0) pthread_cond_broadcast(&_con_cond);
        pthread_mutex_unlock(&_mutex);
        return temp;
    }
    bool empty()
    {
        return _q.size()==0;
    }
    bool full()
    {
        return _q.size()==_cap;
    }
    ~blockqueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_con_cond);
        pthread_cond_destroy(&_pro_cond);
    }
private:
    std::queue<T> _q;
    pthread_mutex_t _mutex;
    pthread_cond_t _con_cond;
    pthread_cond_t _pro_cond;
    int _con_wait_num=0;
    int _pro_wait_num=0;
    int _cap;
};
