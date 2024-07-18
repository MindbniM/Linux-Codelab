#include"socket.hpp"
#include"sys/stat.h"
#include<thread>
#include<vector>
#include<sstream>
#include<fstream>
#include<unordered_map>
const std::string space=" ";
const std::string sep="\r\n";
const std::string head_sep=": ";
const std::string Path="wwwroot";
const std::string suffix_sep=".";
const std::string default_home="/index.html";
class reqline
{
public:
    reqline()
    {
        _map[".html"]="text/html";
        _map[".ico"]="image/x-icon";
        _map[".png"]="image/png";
        _map[".css"]="text/css";
    }
    void Deserialize(const std::string& str)
    {
        std::stringstream s(str);
        s>>_math>>_uri>>_version;
        _path=Path;
        if(_uri[_uri.size()-1]=='/')
        {
            _path+=default_home;
        }
        else 
        {
            _path+=_uri;
        }
        int n=_uri.rfind(suffix_sep);
        if(n==std::string::npos)
        {
            _suffix=".html";
        }
        else 
        {
            _suffix=_uri.substr(n);
        }
        INFO("client wang to %s\n",_path.c_str());
    }
    void prin()
    {
        std::cout<<_math<<" "<<_uri<<" "<<_version<<std::endl;
    }
    std::string serialize()
    {
        std::string str=_version;
        str+=space;
        str+=_code;
        str+=space;
        str+=_codes;
        str+=sep;
        return str;
    }
    void set_response(const std::string& version,const std::string&code,const std::string& codes)
    {
        _version=version;
        _code=code;
        _codes=codes;
    }
    std::string path()
    {
        return _path;
    }
    std::string suffix()
    {
        return _map[_suffix];
    }
private:
    std::string _code;
    std::string _codes;
    std::string _math;
    std::string _uri;
    std::string _version;
    std::string _path;
    std::string _suffix;
    std::unordered_map<std::string,std::string> _map;
};
struct reqhead
{
    reqhead()
    {
    }
    bool Deserialize(std::string& str)
    {
        std::string s;
        auto n=str.find(sep);
        s=str.substr(0,n);
        while(n!=std::string::npos&&!s.empty())
        {
            _addhead(s);
            str.erase(0,n+sep.size());
            n=str.find(sep);
            s=str.substr(0,n);
        }
        return true;
    }
    void _addhead(const std::string& str)
    {
        _reqh.emplace_back(str);
        auto n=str.find(head_sep);
        if(n!=std::string::npos)
        {
            _kv[str.substr(0,n)]=str.substr(n+head_sep.size());
        }
    }
    void add_kv(const std::string& k,const std::string& v)
    {
        _kv[k]=v;
        _reqh.emplace_back(k+head_sep+v+sep);
    }
    void prin()
    {
        for(auto& i:_kv)
        {
            std::cout<<"###"<<i.first<<"###"<<i.second<<std::endl;
        }
    }
    std::string serialize()
    {
        std::string str;
        for(auto& i:_reqh)
        {
            str+=i;
        }
        return str;
    }
    std::vector<std::string> _reqh;
    std::unordered_map<std::string,std::string> _kv;
};
class http_request
{
public:
    http_request():_nline(sep)
    {}
    bool Deserialize(std::string& str)
    {
        if(str.empty()) return false;
        auto n=str.find(sep);
        if(n==std::string::npos) return false;
        _reql.Deserialize(str.substr(0,n));
        str.erase(0,n+sep.size());
        if(str.empty()) return false;
        _reqh.Deserialize(str);
        str.erase(0,sep.size());
        _data=str;
        return true;
    }
    void prin()
    {
        _reql.prin();
        _reqh.prin();
        std::cout<<"这是空行"<<_nline;
        std::cout<<"这是数据"<<_data<<std::endl;
    }
    std::string path()
    {
        return _reql.path();
    }
    std::string suffix()
    {
        return _reql.suffix();
    }
private:
    reqline _reql;
    reqhead _reqh;
    std::string _nline;
    std::string _data;
};
class http_response
{
public:
    http_response():_nline(sep)
    {}
    void add_reql(const reqline& reql)
    {
        _reql=reql;
    }
    void add_reqh(const reqhead& reqh)
    {
        _reqh=reqh;
    }
    int add_data(const std::string& path)
    {
        struct stat stat_buff;
        int rc=stat(path.c_str(),&stat_buff);
        if(rc==-1)
        {
            std::cout<<strerror(errno)<<std::endl;
        }
        int size=stat_buff.st_size;
        std::ifstream file(path,std::ios::binary);
        if(!file.is_open()) return 0;
        _data.resize(size);
        file.read((char *)_data.c_str(),size);
        return size;
    }
    std::string serialize()
    {
        std::string str=_reql.serialize();
        str+=_reqh.serialize();
        str+=_nline;
        str+=_data;
        return str;
    }
    

private:
    reqline _reql;
    reqhead _reqh;
    std::string _nline;
    std::string _data;
};
class http_server : public Socket::Tcp_server
{
public:
    http_server(int prot)
    {
        Tcp_server_init(prot);
    }
    void start()
    {
        while(1)
        {
            inaddr in;
            Tcp_client_ptr p=socket_accept(in);
            if(p==nullptr) continue;
            std::thread t1(&http_server::_start,this,p);
            t1.detach();
        }
    }
    void _start(Tcp_client_ptr p)
    {
        while(1)
        {
            http_request req;
            std::string str;
            Recv(p,str);
            req.Deserialize(str);
            if(req.suffix()=="image/x-icon") continue;
            std::string response=bulid_response(req.path(),req.suffix());
            std::cout<<response<<std::endl;
            Send(p,response);
        }
    }
    std::string bulid_response(const std::string& path,const std::string& suffix)
    {
        http_response re;
        int n=re.add_data(path);
        reqline l;
        l.set_response("HTTP/1.0","200","OK");
        re.add_reql(l);
        reqhead h;
        h.add_kv("Content-Length",std::to_string(n));
        h.add_kv("Content-Type",suffix);
        re.add_reqh(h);
        return re.serialize();
    }
};