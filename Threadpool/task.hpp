#pragma once
class task
{
public:
    template<class Fn,class ...Args>
    task(const std::string& name,Fn&& func,Args ...args):_task_name(name),_func([=](){std::invoke(func,args...);})
    {}
    void task_start()
    {
        _func();
        _task_out=_task_name+"out...";
    }
    std::string out()
    {
        return _task_out;
    }
    void operator()()
    {
        task_start();
    }
private:
    std::string _task_name;
    std::function<void()> _func;
    std::string _task_out;
};

class tasks
{
public:
    tasks()
    {}
    void push(const task& data)
    {
        _q.push(data);
    }
    void pop()
    {
        _q.pop();
    }
    task front()
    {
        return _q.front();
    }
    bool empty()
    {
        return _q.empty();
    }
private:
    std::queue<task> _q;
};