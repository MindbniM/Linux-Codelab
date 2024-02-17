#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
    char str[101];
    memset(str,'\0',100);
    for(int i=0;i<100;i++)
    {
        printf("%s\r",str);
        fflush(stdout);
        usleep(100000);
        str[i]='#';
    }
    printf("\n");
    return 0;
}
