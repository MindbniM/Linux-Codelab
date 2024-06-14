#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include "Thread.hpp"
#include "task.hpp"
#include "log.hpp"
class Threadpool
{
public:
    Threadpool(int thread_num,tasks& tasks) : _thread_num(thread_num), _v(thread_num),_q(tasks)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        pthread_cond_init(&_wcond, nullptr);
        for (int i = 0; i < _thread_num; i++)
        {
            _v[i] = Thread("thread-" + std::to_string(i),&Threadpool::work,this,&_v[i]);
        }
    }
    void start()
    {
        _isrunning=true;
        for(auto& i:_v)
        {
            i.start();
        }
    }
    void stop()
    {
        _isrunning=false;
        if(_wait_num>0&&_q.empty()) 
        pthread_cond_broadcast(&_cond);
    }
    void quit()
    {
        _isquit=true;
        pthread_cond_broadcast(&_cond);
    }
    ~Threadpool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
        for(auto& i:_v)
        {
            i.join();
        }
    }

private:
    int _thread_num;
    std::vector<Thread> _v;
    tasks& _q;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    pthread_cond_t _wcond;
    int _wait_num=0;
    int _s_num=0;
    bool _isrunning = false;
    bool _isquit=false;

    void work(Thread*p)
    {
        while (1)
        {

            pthread_mutex_lock(&_mutex);
            //std::cout<<p->get_thread_name()<<std::endl;
            while(_isrunning && _q.empty())
            {
                _wait_num++;
                //std::cout<<p->get_thread_name()<<"...sleep"<<std::endl;
                pthread_cond_wait(&_cond,&_mutex);
                _wait_num--;
            }
            if ((!_isrunning&&_q.empty())||(_isquit))
            {
                //std::cout<<p->get_thread_name()<<"...quit"<<std::endl;
                INFO("线程%s:quit...\n",p->get_thread_name().c_str());
                pthread_mutex_unlock(&_mutex);
                break;
            }
            //std::cout<<p->get_thread_name()<<"......."<<std::endl;
            INFO("线程%s:running...\n",p->get_thread_name().c_str());
            task temp = _q.front();
            _q.pop();
            if(!_q.empty()&&_wait_num>0)
            pthread_cond_signal(&_cond);
            pthread_mutex_unlock(&_mutex);
            temp();
            sleep(1);
        }
    }
};