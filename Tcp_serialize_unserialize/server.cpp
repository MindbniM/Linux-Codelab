#include"server.hpp"
int main()
{
    server s;
    s.init(8848);
    sleep(5);
    s.start(protocol_ns::Decode,protocol_ns::Encode);
    return 0;
}