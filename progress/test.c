#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main()
{
    char str[102];
    memset(str,'\0',100);
    const char* s="-\\|/";
    pid_t a=fork();
    if(a==0)
    {
        for(int i=0;i<=100;i++)
        {
            printf("[%-100s][%3d%%][%c][Pid:%d][PPid:%d]\r",str,i,s[rand()%5],getpid(),getppid());  
            fflush(stdout);
            usleep(100000);
            str[i]='#';
        }
    }
    else 
    {
        for(int i=0;i<=100;i++)  
        {  
              printf("[%-100s][%3d%%][%c][Pid:%d][PPid:%d]\r",str,i,s[rand()%5],getpid(),getppid());  
              fflush(stdout);  
              usleep(100000);  
              str[i]='#';  
        }  
  
    }
    printf("\n");
    return 0;
}
