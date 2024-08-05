#include"Listen.hpp"
int main()
{
    ADD_APPENDER_STDOUT(DEBUG);
    Listen l(8848);
    Reactor re;
    re.insert(l.fd(),"r",l.addr(),std::bind(&Listen::accept,&l,std::placeholders::_1),nullptr,nullptr);
    re.Dispatcher();
    return 0;
}