#include<iostream>
class inaddr
{
public:
    inaddr(uint32_t ip,uint16_t prot):_ip(ip),_prot(prot)
    {}
    inaddr()
    {}
    inaddr(const std::string& ip,uint16_t prot):_prot(prot)
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
        std::string str;
        struct in_addr temp;
        memset(&temp,0,sizeof(temp));
        temp.s_addr=_ip;
        str=inet_ntoa(temp);
        return str; 
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