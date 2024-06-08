#include"Threadpool.hpp"
int main()
{
    tasks t;
    for(int i=0;i<1000;i++)
    {
        t.push(task("task-0",[i](){std::cout<<std::to_string(i)<<std::endl;}));
    }
    Threadpool tp(5,t);
    tp.start();
    int n=10;
    while(n)
    {
        sleep(1);
        n--;
    }
    tp.stop();
    return 0;
}