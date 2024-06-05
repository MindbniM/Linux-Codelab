#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#include<map>
#include"Thread.hpp"
#include"task.hpp"
#include"blockqueue.hpp"


class cp_thread
{
public:
    cp_thread(int consumer_num=1,int productor_num=1):_consumer_num(consumer_num),_productor_num(productor_num)
    {
        _con_tid.resize(consumer_num);
        _pro_tid.resize(productor_num);
    }
    bool start()
    {
        for(int i=0;i<_consumer_num;i++)
        {
            _con_tid[i]=Thread<cp_thread*>([=](cp_thread*p){c_thread(p,i);},this,"con_thread_"+std::to_string(i));
            _con_tid[i].start();
        }
        for(int i=0;i<_productor_num;i++)
        {
            _pro_tid[i]=Thread<cp_thread*>([=](cp_thread*p){p_thread(p,i);},this,"pro_thread_"+std::to_string(i));
            _pro_tid[i].start();
        }
        return true;
    }
    ~cp_thread()
    {
        for(auto& i:_con_tid)
        {
            i.join();
        }
        for(auto& i:_pro_tid)
        {
            i.join();
        }
    }
private:
    int _consumer_num;
    int _productor_num;
    blockqueue<task>  _q;
    std::vector<Thread<cp_thread*>> _con_tid;
    std::vector<Thread<cp_thread*>> _pro_tid;
    static int temp;
    static void c_thread(cp_thread* p,int i)
    {
        while(true)
        {
            p->_q.push(task([=](){std::cout<<"task---"<<std::to_string(temp)<<"running"<<std::endl,temp++;}));
            //std::cout<<p->_con_tid[i].get_thread_name()<<":"<<"consumer......"<<std::endl;
        }
    }
    static void p_thread(cp_thread* p,int i)
    {
        while(true)
        {
            sleep(3);
            auto num=p->_q.pop();
            num.task_start();
            //std::cout<<p->_pro_tid[i].get_thread_name()<<":"<<"productor......"<<std::endl;
        }
    }
};
int cp_thread::temp=1;