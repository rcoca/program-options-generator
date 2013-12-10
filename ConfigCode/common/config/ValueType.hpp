#ifndef ValueType_hpp
#define ValueType_hpp
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/variant.hpp>
#include "ConfUtils.hpp"
#include "endpoint.hpp"
#include "endpoint_list.hpp"
#include "path_list.hpp"




class ValueType 
{
private:
    boost::variant<bool, double, endpoint, endpoint_list, int, path_list, std::string, url_list, url_string, vector_double > m_data;

    ValueType () ;

public:
    ValueType (bool const & val_bool) ;

    ValueType (double const & val_double) ;

    ValueType (endpoint const & val_endpoint) ;

    ValueType (endpoint_list const & val_endpoint_list) ;

    ValueType (int const & val_int) ;

    ValueType (path_list const & val_path_list) ;

    ValueType (std::string const & val_string) ;

    ValueType (url_list const & val_url_list) ;

    ValueType (url_string const & val_url_string) ;

    ValueType (vector_double const & val_vector_double) ;

    bool get_bool () ;

    double get_double () ;

    endpoint get_endpoint () ;

    endpoint_list get_endpoint_list () ;

    int get_int () ;

    path_list get_path_list () ;

    std::string get_string () ;

    url_list get_url_list () ;

    url_string get_url_string () ;

    vector_double get_vector_double () ;

    friend std::ostream & operator << (std::ostream& os, const ValueType & T) ;

    virtual  ~ValueType () ;
};

#endif /*#ifdef ValueType_hpp*/
