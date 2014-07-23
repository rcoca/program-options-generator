#ifndef ConfigMap_hpp
#define ConfigMap_hpp
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

    std::string FileName;

    int m_argc;

    char ** m_argv;

    boost::shared_ptr<ValueTypeMap >  m_map;

    boost::shared_ptr<OptionsParser>  OptParser;

public:
    typedef ValueTypeMap::iterator iterator;

    typedef ValueTypeMap::const_iterator const_iterator;

    ConfigMap () ;

    ConfigMap (OptionsParser *Parser, const char *xConfigFile, int argc, char *argv[]) ;

    bool ReloadConfig () ;

    bool ReloadConfig (std::string const &xConfigFile) ;

    ConfigMap::iterator begin () ;

    ConfigMap::iterator end () ;

    size_t erase (std::string const & key) ;

    boost::mutex & getLock () ;

    std::pair<ValueTypeMap::iterator,bool> insert (std::pair<std::string,ValueType> val) ;

    friend std::ostream & operator << (std::ostream& os, ConfigMap & cmap) ;

    ValueType operator [] (const char* key) ;

    ValueType operator [] (std::string const & key) ;

    void set (const char* key,ValueType const& v) ;

    size_t size () ;

    virtual  ~ConfigMap () ;
};

#endif /*#ifdef ConfigMap_hpp*/
