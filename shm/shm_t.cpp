#include"fifo.hpp"
#include"shm.hpp"
const int Nnum=10;
const int N=sizeof(int)*Nnum;
const std::string path="/home/lf/code/shm";
int main()
{
    shm<int, N> sh;
    sh.shm_get(path);
    sh.shm_at();
    fifo pipe;
    pipe.Ropen();
    pipe.Read();
    for(int i=0;i<Nnum;i++)
    {
        std::cout<<"reda..."<<sh[i]<<std::endl;
    }
    sleep(5);
    return 0;
}