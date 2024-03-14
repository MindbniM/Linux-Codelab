#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
void command()
{
    const char* s=getenv("USER");
    const char* h=getenv("HOSTNAME");
    const char* bin=getenv("PWD");
    printf("[%s@%s %s]$ ",s,h,bin);
}
#define SIZE 1024
#define MAX_COM 64
void strto(char* argv[])
{
    
    char buff[SIZE];
    fgets(buff,SIZE,stdin);
    buff[strlen(buff)-1]=0;
    int i=0;
    argv[i++]=strtok(buff," ");
    while(argv[i++]=strtok(NULL," "));


}
void exe(char* argv[])
{
    pid_t id=fork();
    if(id==0)
    {
        execvp(argv[0],argv);
        exit(0);
    }
    waitpid(id,NULL,0);
}
int main()
{
    while(1)
    {
        char* argv[MAX_COM];
        
        command();
        strto(argv);
        exe(argv); 
        return 0;
    }
}
