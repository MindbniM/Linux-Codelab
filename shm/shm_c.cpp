#include"fifo.hpp"
#include"shm.hpp"
const int Nnum=10;
const int N=sizeof(int)*Nnum;
const std::string path="/home/lf/code/shm";
int main()
{
    shm<int, N> sh;
    sh.shm_creat(path);
    sh.shm_at();
    fifo pipe;
    pipe.creat();
    pipe.Wopen();
    for(int i=0;i<Nnum;i++)
    {
        sh[i]=i;
        sleep(1);
        std::cout<<"wirte....."<<std::endl;
    }
    pipe.Write();
    sleep(5);
    return 0;
}