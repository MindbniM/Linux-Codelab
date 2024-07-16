#include"client.hpp"
int main()
{
    client c;
    inaddr server("47.121.132.64",htons(8848));
    c.init(server);
    c.start(protocol_ns::Decode,protocol_ns::Encode);
    return 0;
}