#ifndef __SCHEDULE__
#define __SCHEDULE__
#include"Fiber.hpp"
namespace MindbniM
{
    
    struct ScheduleTask
    {
        Fiber::ptr fiber;         //可使用协程调度
        std::function<void()> cb; //可使用函数调用
        ScheduleTask(Fiber::ptr f)
        {
            fiber=f;
        }
        ScheduleTask(std::function<void()> func)
        {
            cb=func;
        }
        void reset()
        {
            fiber=nullptr;
            cb=nullptr;
        }
    };
    class Schedule
    {
        using ptr=std::shared_ptr<Schedule>;
    public:      
        Schedule(size_t thread_num,bool userCall,const std::string& name);
    private:
        std::string m_name;                           //调度器id
        std::mutex m_mutex;                           //互斥锁
        std::queue<ScheduleTask> m_task;              //任务队列
        std::vector<std::thread> m_threads;           //线程池
        int m_threadCount=0;                          //工作线程数
        std::atomic<size_t> m_activeThreadCount={0};  //活跃线程数
        std::atomic<size_t> m_idleThreadCount={0};    //idle线程数
        bool m_userCall;                              //是否将当前调度器所在线程调度
        Fiber::ptr m_rootFiber;                       //如果启用userCall, 当前线程的调度协程
        bool m_stop;                                  //停止调度器
    };
    //当前线程的调度器
    thread_local Schedule* t_schedule=nullptr;
    //当前线程的调度协程
    thread_local Fiber* t_schedule_fiber=nullptr;


    Schedule::Schedule(size_t thread_num,bool userCall,const std::string& name)
    {
        
    }
}
#endif
