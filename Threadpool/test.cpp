#include"Threadpool.hpp"
#include"log.hpp"
int main()
{
    tasks t;
    for(int i=0;i<1000;i++)
    {
        t.push(task("task-0",[i](){std::cout<<std::to_string(i)<<std::endl;}));
    }
    Threadpool tp(5,t);
    INFO("线程池创建\n");
    tp.start();
    int n=10;
    while(n)
    {
        sleep(1);
        n--;
    }
    tp.quit();
    INFO("线程池销毁\n");
    return 0;
}