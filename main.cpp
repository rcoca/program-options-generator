#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

#include "ConfigMap.hpp"
#include "NodeOptions.hpp"

int main(int argc, char *argv[])
{

    try
    {

        ConfigMap MyConfigMap(new NodeOptions(),std::string("node.config"),argc,argv);
        std::cerr<<"Map Size:"<<MyConfigMap.size()<<std::endl;

        try
        {
            MyConfigMap["debug"];
        }
        catch(std::runtime_error &e)
        {
            std::cerr<<"Started"<<std::endl;
        }
        std::cerr<<"Map Size:"<<MyConfigMap.size()<<std::endl;
        for(ConfigMap::iterator i=MyConfigMap.begin(); i!=MyConfigMap.end(); i++)
        {
            std::cerr<<(i->first)<<" : "<<(i->second)<<std::endl;
        }

    }
    catch(std::exception &e)
    {

        std::cerr<<e.what()<<std::endl;
    }


    return 0;
}
