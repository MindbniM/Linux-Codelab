
struct user
{
    user(const std::string& ip,const std::string& prot)
    {
        _name=ip+":"+prot;
    }
    user(uint32_t ip,uint16_t prot):_ip(ip),_prot(prot)
    {
        in_addr temp;
        temp.s_addr=ip;
        char buff[20]={0};
        inet_ntop(AF_INET,&temp,buff,INET_ADDRSTRLEN);
        prot=ntohs(prot);
        _name=std::string(buff)+":"+std::to_string(prot);
    }
    bool operator<(const user& u) const
    {
        return _name<u._name;
    }
    std::string _name;
    uint32_t _ip;
    uint16_t _prot;
};
class users
{
public:
    typedef std::set<user>::iterator iterator;
    void add_user(const user& u)
    {
        _set.insert(u);
    }
    template<class ...Args>
    void add_user(Args&& ...args)
    {
        _set.emplace(std::forward<Args>(args)...);
    }
    void quit_user(const user&u)
    {
        if(_set.count(u))
        {
            _set.erase(u);
        }
    }
    iterator begin()
    {
        return _set.begin();
    }
    iterator end()
    {
        return _set.end();
    }

private:
    std::set<user> _set;
};
