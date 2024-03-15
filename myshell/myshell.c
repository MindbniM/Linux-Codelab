#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define SIZE 1024
#define MAX_COM 64
char pwd[SIZE];
const char* ENVname(const char* name)
{
    const char* s=getenv(name);
    if(s==NULL) return "None";
    return s;
}
void command()
{
    printf("[%s@%s %s]$ ",ENVname("USER"),ENVname("HOSTNAME"),ENVname("PWD"));
}
int strto(char* argv[],char* buff)
{
    
    fgets(buff,SIZE,stdin);
    buff[strlen(buff)-1]=0;
    int i=0;
    argv[i++]=strtok(buff," ");
    while(argv[i++]=strtok(NULL," "));
    if(strcmp(argv[0], "ls") ==0)
    {
        argv[i-1] = (char*)"--color";
        argv[i] = NULL;
    }
    return strlen(buff);

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
int built(char* argv[])
{
    int ret=1;
    if(strcmp(argv[0],"cd")==0)
    {
        ret=0;
        const char* tar=argv[1];
        if(tar==NULL)
        {
            tar=ENVname("HOME");
        }
        chdir(tar);
        char temp[SIZE];
        getcwd(temp,SIZE);
        snprintf(pwd,SIZE,"PWD=%s",temp);
        putenv(pwd);
    }
    return ret;
}
int main()
{
    while(1)
    {
        char* argv[MAX_COM];
        
        char buff[SIZE];
        command();
        int n= strto(argv,buff);
        if(n==0) continue;
        n=built(argv);
        if(n==0) continue;
        exe(argv); 
    }
        return 0;
}
