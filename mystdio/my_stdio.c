
#include"my_stdio.h"
my_FILE* my_fopen(const char* filename,const char* mode)
{
    int flags=0;
    if(strcmp("w",mode)==0)
    {
        flags|=(O_WRONLY|O_CREAT|O_TRUNC);
    }
    else if(strcmp("r",mode)==0)
    {
        flags|=O_RDONLY; 
    }
    else if(strcmp("a",mode)==0)
    {
        flags|=(O_WRONLY|O_CREAT|O_APPEND);
    }
    else return NULL;
    int fd;
    if(flags&O_RDONLY)
    {
        fd=open(filename,flags);
    }
    else
    {
        fd=open(filename,flags,0666);
    }
    if(fd<0) return NULL;
    my_FILE* f=(my_FILE*)malloc(sizeof(my_FILE));
    if(f==NULL) return NULL;
    f->fd=fd;
    f->outpos=0;
    f->inpos=0;
    return f;
}

int my_fclose(my_FILE* stream)
{
    if(stream->outpos!=0)
    {
        ssize_t n=write(stream->fd,stream->outbuffer,stream->outpos);
        if(n<0) return -1;
    }
    close(stream->fd);
    free(stream);
}

size_t my_fwrite(const char* ptr,size_t size,my_FILE* stream)
{
    if(ptr==NULL) return 0;
    size_t n=size;
    while(size!=0)
    {
        if(stream->outpos==MAX_BUFFSIZE-1)
        {
            write(stream->fd,stream->outbuffer,stream->outpos);
            stream->outpos=0;
        }
        stream->outbuffer[stream->outpos++]=*ptr++;
        size--;
    }
    return size;
}

size_t my_fread(char* ptr,size_t size,my_FILE* stream)
{
    if(ptr==NULL) return 0;
    if(stream->inpos==0)
    {
        ssize_t n=read(stream->fd,stream->inbuffer,MAX_BUFFSIZE-1);
        if(n<0) return 0;
    }
    while(size!=0)
    {
        strncpy(ptr,stream->inbuffer,size);
    }
    return size;

}