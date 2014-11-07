#ifndef url_list_hpp
#define url_list_hpp
#include <string>
#include <vector>
#include "url_string.hpp"




struct url_list 
{
public:
    std::vector<url_string> list;

    url_list ()   ;

    url_list (std::vector<url_string> const& urls)   ;
};

#endif /*#ifdef url_list_hpp*/
