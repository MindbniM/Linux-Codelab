#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<cstdlib>
using namespace std;
const char* fifoname="fifo";
int main()
{
    int n=mkfifo(fifoname,0666);
    int fd=open(fifoname,O_WRONLY);
    if(fd<0) exit(1);
    while(true)
    {
        string str;
        getline(cin,str);
        if(str=="q") break;
        write(fd,str.c_str(),str.size());
    }
    close(fd);
    return 0;
}