#ifndef CONFUTILS_HPP
#define CONFUTILS_HPP
#include <string>
#include <iostream>
#include <vector>
#include <boost/variant.hpp>
#include <boost/any.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "endpoint.hpp"
#include "endpoint_list.hpp"
#include "path_list.hpp"




template<typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& v)
{
    typename std::vector<T>::const_iterator i=v.begin();
    os<<"[ ";
    for(;i!=v.end();++i)
        os<<*i<<(i+1==v.end()?" ]":", ");
    return os;
}

std::ostream & operator<< (std::ostream & os, const endpoint& ep);
std::ostream & operator<< (std::ostream& os, const endpoint_list& elist);
std::ostream & operator<< (std::ostream& os, const path_list& plist);
void validate (boost::any& value, const std::vector<std::string> & values, std::vector<std::string>* target_type, int);
void validate (boost::any& value, const std::vector<std::string> & values, std::vector<double>* target_type, int);
void validate (boost::any& value, const std::vector<std::string>& values, endpoint* target_type, int);
void validate (boost::any& value, const std::vector<std::string>& values, path_list* target_type, int);
void validate (boost::any& value, const std::vector<std::string>& values, endpoint_list* target_type, int);

#endif /*#ifdef CONFUTILS_HPP*/
