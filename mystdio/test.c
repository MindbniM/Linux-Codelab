#include"my_stdio.h"
int main()
{
    my_FILE*file=my_fopen("log.txt","a");
    if(file==NULL) exit(-1);
    const char* str="111111111111111111111111\n";
    my_fwrite(str,strlen(str),file);
    my_fclose(file);
    return 0;
}