#include"cp_thread.hpp"
int main()
{
    cp_thread cp(3,3);
    cp.start();
    for(;;);
    return 0;
}