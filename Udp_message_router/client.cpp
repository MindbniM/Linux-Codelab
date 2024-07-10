#include<pthread.h>
#include "client.hpp"
bool flag = true;
bool running = true;
int fd;
sockaddrin _server("47.121.132.64", htons(8848));
struct sockaddr_in server;
void* recvtoserver(void*)
{
    while (running)
    {
        char buff[1024] = {0};
        struct sockaddr_in temp;
        memset(&temp, 0, sizeof(temp));
        socklen_t len = sizeof(temp);
        int n = recvfrom(fd, buff, 1024, 0, (struct sockaddr *)&temp, &len);
        if (n > 0)
        {
            buff[n] = 0;
            fprintf(stderr, "%s", buff);
        }
    }
    return NULL;
}
void* sendtoserver(void*)
{
    while (running)
    {
        std::string str;
        if (flag)
        {
            str = "join in";
            flag = false;
        }
        else
        {
            std::cout << "get string\n";
            getline(std::cin, str);
            if (str == "quit")
                running = false;
            std::cout<<running<<std::endl;
        }
        sendto(fd, str.c_str(), str.size(), 0, (struct sockaddr *)&server, sizeof(server));
    }
    return NULL;
}
int main()
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        FATAL("socket error\n");
        exit(1);
    }
    INFO("socker succsee\n");
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = _server.inet_ip();
    server.sin_port = _server.inet_prot();
    pthread_t p1;
    pthread_t p2;
    pthread_create(&p2,NULL,sendtoserver,NULL);
    pthread_create(&p1,NULL,recvtoserver,NULL);
    pthread_join(p2,NULL);
    pthread_join(p1,NULL);
    return 0;
}