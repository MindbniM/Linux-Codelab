#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
using namespace std;
const char* fifoname="fifo";
int main()
{
    int fd=open(fifoname,O_RDONLY);
    if(fd<0) exit(1);
    while(true)
    {
        char buff[64]={0};
        ssize_t n=read(fd,buff,63);
        if(n>0)
        {
            buff[n]=0;
            printf("%s\n",buff);
        }
        else if(n==0)
        {
            break;
        }
        else 
        {
            printf("error :%s",strerror(errno));
        }
    }
    close(fd);
    return 0;
}