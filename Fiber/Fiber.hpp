#pragma once
#include<memory>
#include<string>
#include<functional>
#include<thread>
#include<ucontext.h>
namespace MindbniM
{
    class Fiber : public std::enable_shared_from_this<Fiber>
    {
    public:
        using func_t=std::function<void()>;
        using ptr=std::shared_ptr<Fiber>;
        enum class state
        {
          INIT,   //初始化
          HOLD,   //挂起
          EXEC,   //运行
          TERM,   //结束
          READY   //就绪
        };
    public:
        Fiber(func_t cb,uint32_t stack_size=0);
        ~Fiber();

        //重置协程函数,重置状态
        void reset(func_t cb);
        //切换到当前协程运行
        void swapIn();
        //切换到后台运行
        void swapOut();
    public:
        //获取当前协程的智能指针
        static Fiber::ptr Get_this();
        static void YeildtoReady();
        static void YeildtoHold();
    private:
        Fiber();
    private:
        uint64_t m_id=0;            //协程id
        uint32_t m_stack_size=0;    //协程栈空间大小
        ucontext_t m_ctx;           //协程上下文
        void* m_stack=nullptr;      //协程使用的栈
        state m_s=state::INIT;      //协程状态
        func_t m_cb;                //协程具体回调
    };
}
