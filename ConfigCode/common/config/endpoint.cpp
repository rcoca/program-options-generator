
#include "endpoint.hpp"

endpoint::endpoint()
     
{

}
endpoint::endpoint(std::string const& host,unsigned short port_)
    :address(host), port(port_)
{

}
