#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
class sockaddrin
{
public:
    sockaddrin(uint32_t ip,uint16_t prot):_ip(ip),_prot(prot)
    {}
    sockaddrin(const std::string& ip,uint16_t prot):_prot(prot)
    {
        _ip=inet_addr(ip.c_str());
    }
    uint32_t inet_ip()
    {
        return _ip;
    }
    uint16_t inet_prot()
    {
        return _prot;
    }
    std::string s_ip()
    {
        char buff[20]={0};
        struct in_addr temp;
        memset(&temp,0,sizeof(temp));
        temp.s_addr=_ip;
        inet_ntop(AF_INET,&temp,buff,sizeof(temp));
        return buff; 
    }
    std::string s_prot()
    {
        int prot=ntohs(_prot);
        return std::to_string(prot);
    }
private:
    uint32_t _ip;
    uint16_t _prot;

};