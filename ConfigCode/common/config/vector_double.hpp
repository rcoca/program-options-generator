#ifndef vector_double_hpp
#define vector_double_hpp
#include <string>
#include <vector>




struct vector_double 
{
public:
    std::vector<double> values;

    vector_double () ;

    vector_double (std::vector<double> const& data) ;
};

#endif /*#ifdef vector_double_hpp*/
