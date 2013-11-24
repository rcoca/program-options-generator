
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
}
ConfigMap::iterator ConfigMap::begin()
{
   return m_map->begin();
}
ConfigMap::iterator ConfigMap::end()
{
    return m_map->end();
}
boost::mutex & ConfigMap::getLock()
{
    return m_mutex;
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
size_t ConfigMap::size()
{
    boost::mutex::scoped_lock lock(m_mutex);
    return m_map->size();
}
ConfigMap::~ConfigMap()
{

}
