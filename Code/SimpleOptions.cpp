
#include "SimpleOptions.hpp"

bool SimpleOptions::Parse(std::string const& ConfigName,int argc,char *argv[],boost::shared_ptr<ValueTypeMap> pVM)
{

        boost::program_options::options_description description("Simple options");
        path_list  DataDir;
        vector_double  Generations;
        double  InitialTemperature;
        int  PopulationSize;
        endpoint_list  Nodes;
        endpoint  MasterNode;
        url_list  certificates;
        ;
        description.add_options()
        ("help", "help message")
        ("debug","debug run")
        ("DataDir",boost::program_options::value< path_list >(&DataDir) ->multitoken()->required(),"Main DataDir: path_list (ex:/data/)")
        ("Generations",boost::program_options::value< vector_double >(&Generations) ->multitoken(),"Main Generations: floats (ex:50.2,100.0 # optional)")
        ("InitialTemperature",boost::program_options::value< double >(&InitialTemperature) ,"Main InitialTemperature: float (ex:120.0 # optional)")
        ("PopulationSize",boost::program_options::value< int >(&PopulationSize) ->required(),"Main PopulationSize: int (ex:3500 # required)")
        ("Nodes",boost::program_options::value< endpoint_list >(&Nodes) ->multitoken()->required(),"Main Nodes: ip_port_list (ex:192.168.1.3:4900, 192.168.1.4:4900)")
        ("MasterNode",boost::program_options::value< endpoint >(&MasterNode) ->required(),"Main MasterNode: ip_port (ex:192.168.1.110:4900)")
        ("certificates",boost::program_options::value< url_list >(&certificates) ->multitoken()->required(),"Main certificates: url_list (ex:http://node.host.net:44434/cert.pem, ftp://node.host.com/cert1.pem)")
        ;
        boost::program_options::variables_map vars_map;
        boost::program_options::store(boost::program_options::parse_command_line(argc,argv,description),vars_map);
        if(vars_map.count("help"))std::cout<<description<<std::endl;
        if(vars_map.count("debug"))std::cout<<"debug mode"<<std::endl;
        //if config is a usefull parameter in ctor, then use it, else ignore it
        if(ConfigName.size())
        {
            std::ifstream conf(ConfigName.c_str());
            if(conf.is_open())
                boost::program_options::store(boost::program_options::parse_config_file(conf,description),vars_map);
            else
                throw std::runtime_error(ConfigName+":"+std::strerror( errno ));
            //override config with command line
            boost::program_options::store(boost::program_options::parse_command_line(argc,argv,description),vars_map);
        }
        boost::program_options::notify(vars_map);
        pVM->insert(std::make_pair("DataDir", ValueType(DataDir)));
        pVM->insert(std::make_pair("Generations", ValueType(Generations)));
        pVM->insert(std::make_pair("InitialTemperature", ValueType(InitialTemperature)));
        pVM->insert(std::make_pair("PopulationSize", ValueType(PopulationSize)));
        pVM->insert(std::make_pair("Nodes", ValueType(Nodes)));
        pVM->insert(std::make_pair("MasterNode", ValueType(MasterNode)));
        pVM->insert(std::make_pair("certificates", ValueType(certificates)));
        if(vars_map.count("debug"))pVM->insert(std::make_pair("debug",ValueType(1)));
        else pVM->insert(std::make_pair("debug",ValueType(0)));
        return true;
}