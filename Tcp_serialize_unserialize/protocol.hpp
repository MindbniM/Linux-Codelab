#include<jsoncpp/json/json.h>
namespace protocol_ns
{
    std::string Encode(const std::string& json_str)
    {
        std::string str;
        const std::string ep="\r\n";
        str+=std::to_string(json_str.size());
        str+=ep;
        str+=json_str;
        str+=ep;
        return str;
    }
    std::string Decode( std::string& Encode_str)
    {
        const std::string ep="\r\n";
        size_t n=Encode_str.find(ep);
        if(n==std::string::npos) return "";
        std::string len=Encode_str.substr(0,n);
        int size=std::stoi(len);
        int total=size+2*ep.size()+len.size();
        if(Encode_str.size()<total) return "";
        std::string json_str=Encode_str.substr(n+len.size(),size);
        Encode_str.erase(0,total);
        return json_str;

    }
    struct request
    {
        request(int x,int y,int c):_x(x),_y(y),_char(c)
        {}
        request()
        {}
        std::string serialize()
        {
            Json::Value v;
            v["x"]=_x;
            v["y"]=_y;
            v["char"]=_char;
            Json::FastWriter w;
            std::string str=w.write(v);
            return str;
        }
        bool unserialize(std::string& str)
        {
            Json::Value v;
            Json::Reader r;
            bool f=r.parse(str,v);
            if(!f) return false;
            _x=v["x"].asInt();
            _char=v["char"].asInt();
            _y=v["y"].asInt();
            return true;
        }
        int _x;
        int _y;
        int _char;
    };
    struct response
    {
        response()
        {}
        response(int ret,int code=0):_ret(ret),_code(code)
        {}
        std::string serialize()
        {
            Json::Value v;
            v["ret"]=_ret;
            v["code"]=_code;
            Json::FastWriter w;
            return w.write(v);
        }
        bool unserialize(std::string& str)
        {
            Json::Value v;
            Json::Reader r;
            bool f=r.parse(str,v);
            if(!f) return false;
            _ret=v["ret"].asDouble();
            _ret=v["code"].asInt();
            return true;

        }
        int _ret;
        int _code=0;
    };
    
} 
