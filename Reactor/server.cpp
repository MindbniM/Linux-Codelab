#include"Listen.hpp"
int main()
{
    ADD_APPENDER_STDOUT(DEBUG);
    //Listen l(8848);
    //Reactor re;
    //re.insert(l.fd(),"r",l.addr(),std::bind(&Listen::accept,&l,std::placeholders::_1),nullptr,nullptr);
    //re.Dispatcher();
    std::string str="GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0\r\nContent-Length: 0\r\n\r\n";
    std::string s1="GET /search?q=openai HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0\r\nContent-Length: 0\r\n\r\n";
    std::string s2="POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nfield1=value1&field2=value2\r\n";
    std::string s3="GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0\r\nContent-Length: 0\r\n";
    std::string s4="POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nfield1=value1&field2=\r\n";
    std::string s5=str+s4;
    http_service::service(s5);
    s5+="123456";
    http_service::service(s5);
    return 0;
}