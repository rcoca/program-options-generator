#ifndef CONFIGMAP_HPP
#define CONFIGMAP_HPP
#include <string>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include "ConfUtils.hpp"
#include "OptionsParser.hpp"
#include "ValueType.hpp"




class ConfigMap 
{
private:
    boost::mutex m_mutex;

    boost::shared_ptr<ValueTypeMap >  m_map;

    std::string FileName;

    int m_argc;

    char ** m_argv;

    boost::shared_ptr<OptionsParser>  OptParser;

    ConfigMap () ;

public:
    typedef ValueTypeMap::iterator iterator;

    typedef ValueTypeMap::const_iterator const_iterator;

    ConfigMap (OptionsParser *Parser, const char *xConfigFile, int argc, char *argv[]) ;

    bool ReloadConfig () ;

    bool ReloadConfig (std::string const &xConfigFile) ;

    ConfigMap::iterator begin () ;

    ConfigMap::iterator end () ;

    boost::mutex & getLock () ;

    ValueType operator [] (const char* key) ;

    ValueType operator [] (std::string const & key) ;

    size_t size () ;

    virtual  ~ConfigMap () ;
};

#endif /*#ifdef CONFIGMAP_HPP*/
