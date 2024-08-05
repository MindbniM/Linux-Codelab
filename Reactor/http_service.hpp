#pragma once
#include <unordered_map>
#include<sstream>
#include<fstream>
#include<vector>


const std::string space=" ";
const std::string sep="\r\n";
const std::string head_sep="\r\n\r\n";//报头分隔符
const std::string head_s=": ";
const std::string Path="../wwwroot";
const std::string suffix_sep=".";
const std::string default_home="index.html";
const std::string f404=Path+"/404.html";

namespace Head
{
    const std::string Version="HTTP/1.1";
    const std::string Code_200="200";
    const std::string Code_200_str="OK";
    const std::string Code_404="404";
    const std::string Code_404_str="Not Found";
}
//http报头
namespace Header
{
    const std::string Content_Type="Content-Type";
    const std::string Content_Length="Content-Length";
    const std::string Host="Host";
    const std::string User_Agent="User-Agent";
    const std::string Referer = "Referer";
    const std::string Location="Location";
    const std::string Cookie="Cookie";
}
//http请求行
struct http_rehead
{
    void Deserialize(const std::string& str)
    {
        std::istringstream os(str);
        os>>_mathod>>_URI>>_version;
    }
    void debug()
    {
        std::cout<<"请求方法:"<<_mathod<<std::endl;
        std::cout<<"URI:"<<_URI<<std::endl;
        std::cout<<"版本:"<<_version<<std::endl;
    }
    std::string _mathod;
    std::string _URI;
    std::string _version;
};
struct http_line
{
    void Deserialize(const std::string& str)
    {
        int i=0;
        int n=str.find(sep);
        while(n!=std::string::npos)
        {
            int temp=str.find(head_s,i+1);
            _kv[str.substr(i,temp-i)]=str.substr(temp+head_s.size(),n-temp-head_s.size());
            i=n+sep.size();
            n=str.find(sep,n+1);
        }
    }
    std::string Serialize()
    {
        std::string line;
        for(auto&[s1,s2] :_kv)
        {
            line+=s1+head_s+s2+sep;
        }
        return line;
    }
    void debug()
    {
        for(auto&[s1,s2]:_kv)
        {
            std::cout<<"type:"<<s1<<" : "<<"data:"<<s2<<std::endl;
        }
    }
    int data_size()
    {
        if(!_kv.count(Header::Content_Length)) return 0;
        return std::stoi(_kv[Header::Content_Length]);
    }
    void insert(const std::string type,const std::string data)
    {
        _kv[type]=data;
    }
    std::unordered_map<std::string, std::string> _kv;
};
//http请求
class http_request
{
public:
    http_request()=default;
    http_request(const http_rehead& head,const http_line& line,const std::string& data):_head(head),_line(line),_data(data)
    {}
    void Deserialize(const std::string& str)
    {

    }
    void debug()
    {
        std::cout<<"这是一个完整的报文"<<std::endl;
        std::cout<<"这是请求行"<<std::endl;
        _head.debug();
        std::cout<<"这是报头"<<std::endl;
        _line.debug();

        std::cout<<"这是正文"<<std::endl;
        std::cout<<_data<<std::endl;
        std::cout<<"结束"<<std::endl;
    }
    void clear()
    {
        _head._mathod.clear();
        _head._URI.clear();
        _head._version.clear();
        _line._kv.clear();
        _data.clear();
    }
    http_rehead head()
    {
        return _head;
    }
    http_line line()
    {
        return _line;
    }
    std::string data()
    {
        return _data;
    }
private:
    http_rehead _head;
    http_line _line;
    std::string _data; //请求正文
};
//http应答状态行
struct http_rphead
{
    std::string Serialize()
    {
        return _version+" "+_code+" "+_codestr;
    }
    std::string _version;
    std::string _code;
    std::string _codestr;
};
//http应答
class http_rponse
{
public:
    std::string Serialize()
    {
        std::string rp=_head.Serialize();
        rp+=sep;
        rp+=_line.Serialize();
        rp+=sep;
        for(auto& c:_data)
        {
            rp+=c;
        }
        rp+=sep;
        return rp;
    }
    void insert(const std::string type,const std::string data)
    {
        _line.insert(type,data);
    }
    void set_head(const http_rphead& head)
    {
        _head=head;
    }
    void set_data(const std::vector<char>& data)
    {
        _data=data;
    }
static std::unordered_map<std::string,std::string> s_hash;
private:
    http_rphead _head;
    http_line _line;
    std::vector<char> _data;
};
std::unordered_map<std::string,std::string> http_rponse::s_hash{
    { "html", "text/html" },
    { "htm", "text/html" },
    { "css", "text/css" },
    { "xml", "application/xml" },
    { "gif", "image/gif" },
    { "jpeg", "image/jpeg" },
    { "jpg", "image/jpeg" },
    { "js", "application/javascript" },
    { "json", "application/json" },
    { "png", "image/png" },
    { "svg", "image/svg+xml" },
    { "txt", "text/plain" },
    { "woff", "font/woff" },
    { "woff2", "font/woff2" },
    { "ttf", "font/ttf" },
    { "ico", "image/vnd.microsoft.icon" },
    { "mp3", "audio/mpeg" },
    { "mpeg", "video/mpeg" },
    { "mp4", "video/mp4" },
    { "webm", "video/webm" },
    { "ogg", "audio/ogg" },
    { "ogv", "video/ogg" },
    { "pdf", "application/pdf" },
    { "zip", "application/zip" },
    { "rar", "application/x-rar-compressed" }
};
//服务
class http_service
{ 
public:
    //判断一个报文是否完整
    bool static get_full_message(std::string& inbuff,http_request& re)
    {
        //先判断是否报头完整
        if(inbuff.size()==0) return false;
        int n=inbuff.find(head_sep);
        if(n==std::string::npos)
        {
            std::cout<<"缺少报头"<<std::endl;
            return false;
        }
        http_rehead head;
        http_line line;
        int headline=inbuff.find(sep);
        //提取请求行
        head.Deserialize(inbuff.substr(0,headline));
        size_t hbegin=headline+sep.size();
        //提取请求报头
        line.Deserialize(inbuff.substr(hbegin,n-hbegin+sep.size()));
        //从请求报头中获取正文长度
        int data_size=line.data_size();
        //计算这个报文总长度
        int request_size=n+head_sep.size()+data_size;
        //如果有正文还有加上正文的结尾
        if(data_size>0) request_size+=sep.size();
        //判断正文是否完整
        if(inbuff.size()<request_size) 
        {
            std::cout<<"缺少正文"<<std::endl;
            return false;
        }
        std::string data=inbuff.substr(n+head_sep.size(),data_size);
        inbuff.erase(0,request_size);
        re=http_request(head,line,data);
        return true;
    }
    //业务处理 
    void static service(Connect* con)
    {
        std::string& inbuff=con->inbuff();
        http_request re;
        bool temp=get_full_message(inbuff,re);
        while(temp)
        {
            //处理一个报文
            con->outbuff()+=_service(re);
            //re.debug();
            //std::cout<<"已经提取一个完整的报文,缓冲区剩下"<<inbuff.size()<<std::endl;
            re.clear();
            //std::cout<<"re clear"<<std::endl;
            temp=get_full_message(inbuff,re);
        }
    }
    std::string static _service(http_request& re)
    {
        if(re.head()._mathod=="GET")
        {
            std::string str=get_mathod(re);
            //std::cout<<str<<std::endl;
            return str;
        }
        else if(re.head()._mathod=="POST")
        {

        }
        else if(re.head()._mathod=="")
        {

        }
        return "";
    }
    std::string static get_mathod(http_request& re)
    {
        http_rponse rp;
        http_rphead head;
        head._version=Head::Version;
        head._code=Head::Code_200;
        head._codestr=Head::Code_200_str;
        std::string path= Path+re.head()._URI;
        if(path.back()=='/') path+=default_home;
        std::cout<<path<<std::endl;
        int size;
        std::vector<char> data=get_file(path,size,head);
        std::string mime_type=get_mime(path);
        rp.set_head(head);
        rp.set_data(data);
        rp.insert(Header::Content_Type,mime_type);
        rp.insert(Header::Content_Length,std::to_string(size));
        return rp.Serialize();
    }
    std::vector<char> static get_file(const std::string& path,int& size,http_rphead& head)
    {
        std::ifstream file(path.c_str(),std::ios::binary);
        if(!file.is_open())
        {
            head._code=Head::Code_404;
            head._codestr=Head::Code_404_str;
            //std::cout<<404<<std::endl;
            return get_file(f404,size,head);
        }
        file.seekg(0,std::ios::end);
        size=file.tellg();
        file.seekg(0,std::ios::beg);
        char* buff=new char[size];
        if(file.read(buff,size));
        std::vector<char> vc(buff,buff+size);
        delete[] buff;
        return vc;
    }
    std::string static get_mime(const std::string& path)
    {
        int n=path.rfind(suffix_sep);
        if(n==std::string::npos) return "text/html";
        std::string suffix=path.substr(n+suffix_sep.size());
        if(http_rponse::s_hash.count(suffix))
        {
            return http_rponse::s_hash[suffix];
        }
    }
};