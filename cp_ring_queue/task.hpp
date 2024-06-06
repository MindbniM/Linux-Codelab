#pragma once
class task
{
public:
    task(std::function<void()> func,std::string name="task"):_task_name(name),_func(func)
    {}
    void task_start()
    {
        _func();
    }
private:
    std::string _task_name;
    std::function<void()> _func;
};