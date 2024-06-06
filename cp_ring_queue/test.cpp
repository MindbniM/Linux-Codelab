#include"cp_thread.hpp"
const int n=10;
int main()
{
    cp_thread cp(3,3);
    cp.start();
    for(int i=0;i<n;i++);
    {
        sleep(1);
    }
    return 0;
}