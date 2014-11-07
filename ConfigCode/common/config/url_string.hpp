#ifndef url_string_hpp
#define url_string_hpp
#include <string>




struct url_string 
{
public:
    std::string protocol;

    std::string host;

    std::string path;

    url_string ()   ;

    url_string (std::string const& proto, std::string const& host_, std::string const&  path_)   ;
};

#endif /*#ifdef url_string_hpp*/
