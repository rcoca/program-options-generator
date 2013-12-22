#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

#include "ConfigMap.hpp"
//#include "NodeOptions.hpp"
#include "SimpleOptions.hpp"
int main(int argc, char *argv[])
{

    try
    {

        //ConfigMap MyConfigMap(new NodeOptions(),"node.config",argc,argv);
        ConfigMap MyConfigMap(new SimpleOptions(),"../simple.ini",argc,argv);
        std::cerr<<"Map Size:"<<MyConfigMap.size()<<std::endl;

        try
        {
            if(MyConfigMap["debug"].get_int())std::cerr<<"Running in Debug mode"<<std::endl;
        }
        catch(std::runtime_error &e)
        {
            std::cerr<<"Started"<<std::endl;
        }

        if(MyConfigMap.ReloadConfig())
        {
            std::cerr<<"Config Reloaded"<<std::endl<<std::endl;
        }
        {
            boost::mutex::scoped_lock Lock(MyConfigMap.getLock());
            for(ConfigMap::const_iterator i=MyConfigMap.begin(); i!=MyConfigMap.end(); i++)
            {
                std::cerr<<i->first<<" : "<<i->second<<std::endl;

            }
        }

    }
    catch(std::exception &e)
    {

        std::cerr<<"Error, exiting: "<<e.what()<<std::endl;
    }


    return 0;
}
