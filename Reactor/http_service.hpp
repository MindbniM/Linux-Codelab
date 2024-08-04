#pragma once
#include <unordered_map>
#include<sstream>
#include<vector>


const std::string space=" ";
const std::string sep="\r\n";
const std::string head_sep="\r\n\r\n";//报头分隔符
const std::string head_s=": ";
const std::string Path="wwwroot";
const std::string suffix_sep=".";
const std::string default_home="/index.html";

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

    }
    std::string _version;
    int _code;
    std::string _codestr;
};
//http应答
class http_rponse
{
public:
    std::string Serialize()
    {

    }
private:
    http_rphead _head;
    http_line _line;
    std::string _data;
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
    std::string static service(std::string &inbuff)
    {
        http_request re;
        bool temp=get_full_message(inbuff,re);
        while(temp)
        {
            //处理一个报文
            re.debug();
            std::cout<<"已经提取一个完整的报文,缓冲区剩下"<<inbuff.size()<<std::endl;
            re.clear();
            //std::cout<<"re clear"<<std::endl;
            temp=get_full_message(inbuff,re);
        }
        return "";
    }

};