#ifndef endpoint_hpp
#define endpoint_hpp
#include <string>




struct endpoint 
{
public:
    std::string address;

    unsigned short port;

    endpoint ()   ;

    endpoint (std::string const& host, unsigned short port_)   ;
};

#endif /*#ifdef endpoint_hpp*/
