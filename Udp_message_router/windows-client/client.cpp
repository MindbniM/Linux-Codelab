#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<string>
#include<thread>
#include<WinSock2.h>
#include<Windows.h>
using namespace std;
#pragma comment(lib,"ws2_32.lib")



bool flag = true;
bool running = true;
const string ip = "47.121.132.64";
const uint16_t prot = 8848;
SOCKET fd;
struct sockaddr_in server;


void recvtoserver()
{
    while (running)
    {
        char buff[1024] = { 0 };
        struct sockaddr_in temp;
        memset(&temp, 0, sizeof(temp));
        int len = sizeof(temp);
        int n = recvfrom(fd, buff, 1024, 0, (struct sockaddr*)&temp, &len);
        if (n > 0)
        {
            buff[n] = 0;
            printf("%s",buff);
        }
    }
}
void sendtoserver()
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
            getline(std::cin, str);
            if (str == "quit")
                running = false;
        }
        sendto(fd, str.c_str(), str.size(), 0, (struct sockaddr*)&server, sizeof(server));
    }
}
int main()
{
    system("CHCP 65001");
    WSADATA w;
    int ret = WSAStartup(MAKEWORD(2, 2), &w);
    if (ret != 0)
    {
        std::cerr << "WSAStartup failed: " << ret << std::endl;
        return 1;
    }
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == SOCKET_ERROR)
    {
        cout << "socket error" << endl;
        return 1;
    }
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(prot);
    thread t1(recvtoserver);
    thread t2(sendtoserver);
    t1.join();
    t2.join();
    closesocket(fd);
    WSACleanup();
    return 0;

}