#pragma once

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define MAX_BUFFSIZE 4096
typedef struct my_FILE
{
    int fd;
    char outbuffer[MAX_BUFFSIZE];
    char inbuffer[MAX_BUFFSIZE];
    int outpos;
    int inpos;


}my_FILE;

my_FILE* my_fopen(const char* filename,const char* mode);

int my_fclose(my_FILE* stream);

size_t my_fwrite(const char* ptr,size_t size,my_FILE* stream);

size_t my_fread(char* ptr,size_t size,my_FILE* stream);
