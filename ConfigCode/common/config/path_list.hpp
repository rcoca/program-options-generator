#ifndef path_list_hpp
#define path_list_hpp
#include <vector>
#include <string>




struct path_list 
{
public:
    std::vector<std::string> paths;

    path_list ()   ;

    path_list (std::vector<std::string> const& pts)   ;
};

#endif /*#ifdef path_list_hpp*/
