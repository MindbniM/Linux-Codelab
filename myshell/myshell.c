#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>

#define SIZE 1024
#define MAX_COM 64


#define Non_file -1
#define Stdin_file 0
#define Stdout_file 1
#define Append_file 2
int fstype=Non_file;
char* filename=NULL;


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
    fstype=Non_file;
    filename=NULL;
    fgets(buff,SIZE,stdin);
    buff[strlen(buff)-1]=0;
    int i=0;
    argv[i++]=strtok(buff," ");
    while(argv[i++]=strtok(NULL," "));
    int n=0;
    for(n=0;argv[n];n++)
    {
        if(strcmp(argv[n],">")==0)
        {
           fstype=Stdout_file;
           filename=argv[n+1];
           break;
        }
        else if(strcmp(argv[n],"<")==0)
        {
            fstype=Stdin_file;
            filename=argv[n+1];
            break;
        }
        else if(strcmp(argv[n],">>")==0)
        {
            fstype=Append_file;
            filename=argv[n+1];
            break;
        }
    }
    if(fstype!=Non_file)
    {
        argv[n]=NULL;
        i=n+1;
    }
    
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
        if(fstype!=Non_file)
        {
            int fast=0;
            int fd=-1;
            int _fd=0;
            if(fstype==Stdin_file)
            {
               fast|=O_RDWR;
               _fd=0;
            }
            else if(fstype==Stdout_file)
            {
                fast|=(O_WRONLY|O_CREAT|O_TRUNC);
                _fd=1;
            }
            else if(fstype==Append_file)
            {
                _fd=1;
                fast|=(O_WRONLY|O_CREAT|O_APPEND);
            }
            fd=open(filename,fast,0666);
            if(fd<0) exit(-1);
            dup2(fd,_fd);
        }
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
