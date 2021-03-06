#ifndef RequiredOptions_hpp
#define RequiredOptions_hpp
#include "ValueType.hpp"
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include "endpoint.hpp"
#include "path_list.hpp"
#include "endpoint_list.hpp"
#include <cerrno>
#include <cstring>
#include "ConfUtils.hpp"
#include "OptionsParser.hpp"




struct RequiredOptions :public OptionsParser
{
public:
    bool Parse (std::string const& ConfigName, int argc, char *argv[], boost::shared_ptr<ValueTypeMap> pVM)   ;
};

#endif /*#ifdef RequiredOptions_hpp*/
