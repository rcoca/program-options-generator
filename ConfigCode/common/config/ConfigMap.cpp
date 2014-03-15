//
//  ConfigMap.cpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2013-2014 Razvan Coca (razvan dot coca at  gmail dot com)
// This is generated code, no need to edit. The translator is distributed under GPL.
// 
//



#include "ConfigMap.hpp"

ConfigMap::ConfigMap(OptionsParser *Parser,const char *xConfigFile,int argc,char *argv[])
    :FileName(xConfigFile), m_argc(argc), m_argv(argv), m_map(new ValueTypeMap())
{

        OptParser.reset(Parser);
        OptParser->Parse(xConfigFile,argc,argv,m_map);
}
bool ConfigMap::ReloadConfig()
{

        try
        {
            boost::shared_ptr<ValueTypeMap> PtrVariablesMap(new ValueTypeMap());
    
            boost::mutex::scoped_lock lock(m_mutex);
            OptParser->Parse(FileName,m_argc,m_argv,PtrVariablesMap);
            m_map=PtrVariablesMap;
            return true;
        }
        catch(boost::program_options::validation_error &e)
        {
            std::cerr<<"Parse Error["<<FileName<<"]:"<<e.what()<<std::endl;
        }
        catch(std::exception &e)
        {
            std::cerr<<e.what()<<std::endl;
        }
        catch(...)
        {
            std::cerr<<"Unknown Error["<<FileName<<"]"<<std::endl;
        }
        return false;
}
bool ConfigMap::ReloadConfig(std::string const &xConfigFile)
{

            try{
               boost::shared_ptr<ValueTypeMap> PtrVariablesMap(new ValueTypeMap());
               OptParser->Parse(xConfigFile,m_argc,m_argv,PtrVariablesMap);
               
               boost::mutex::scoped_lock lock(m_mutex);
               m_map=PtrVariablesMap;
               FileName=xConfigFile;
               return true;
            }
            catch(boost::program_options::validation_error &e)
            {
                std::cerr<<"Parse Error["<<xConfigFile<<"]:"<<e.what()<<std::endl;
            }
            catch(std::exception &e)
            {
                std::cerr<<e.what()<<std::endl;
            }
            catch(...)
            {
                std::cerr<<"Unknown Error["<<xConfigFile<<"]"<<std::endl;
            }
            return false;
}
ConfigMap::iterator ConfigMap::begin()
{
   return m_map->begin();
}
ConfigMap::iterator ConfigMap::end()
{
    return m_map->end();
}
size_t ConfigMap::erase(std::string const & key)
{

        boost::mutex::scoped_lock lock(m_mutex);
        return m_map->erase(key);
}
boost::mutex & ConfigMap::getLock()
{
    return m_mutex;
}
std::pair<ValueTypeMap::iterator,bool> ConfigMap::insert(std::pair<std::string,ValueType> val)
{

        boost::mutex::scoped_lock lock(m_mutex);
        return m_map->insert(val);
}
std::ostream & operator <<(std::ostream& os,ConfigMap & cmap)
{

        boost::mutex::scoped_lock lock(cmap.m_mutex);
        for(ConfigMap::iterator i=cmap.begin();i!=cmap.end();i++)
            os<<i->first<<" = "<<i->second<<std::endl;
        return os;
}
ValueType ConfigMap::operator [](const char* key)
{
    return (*this)[std::string(key)];
}
ValueType ConfigMap::operator [](std::string const & key)
{

        boost::mutex::scoped_lock lock(m_mutex);
        ValueTypeMap::const_iterator i=m_map->find(key);
        if(i!=m_map->end())return i->second;
        throw std::runtime_error(std::string("key:")+key+" not found");
}
void ConfigMap::set(const char* key,ValueType const& v)
{
    boost::mutex::scoped_lock lock(m_mutex);
    m_map->erase(key);
    m_map->insert(std::make_pair(std::string(key),v));
}
size_t ConfigMap::size()
{
    boost::mutex::scoped_lock lock(m_mutex);
    return m_map->size();
}
ConfigMap::~ConfigMap()
{

}
