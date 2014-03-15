//
//  OptionsParser.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2013-2014 Razvan Coca (razvan dot coca at  gmail dot com)
// This is generated code, no need to edit. The translator is distributed under GPL.
// 
//


#ifndef OptionsParser_hpp
#define OptionsParser_hpp
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include "endpoint.hpp"
#include "endpoint_list.hpp"
#include "path_list.hpp"
#include "ValueType.hpp"


typedef std::map<std::string,ValueType> ValueTypeMap;

class OptionsParser 
{
public:
    virtual bool Parse (std::string const& ConfigName, int argc, char *argv[], boost::shared_ptr<ValueTypeMap> pVM)     = 0;
};

#endif /*#ifdef OptionsParser_hpp*/
