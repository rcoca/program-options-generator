
#include "SimpleOptions.hpp"

bool SimpleOptions::Parse(std::string const& ConfigName,int argc,char *argv[],boost::shared_ptr<ValueTypeMap> pVM)
     
{

        boost::program_options::options_description description("Simple options");
        std::string  DataDir;
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
        ("DataDir,d",boost::program_options::value< std::string >(&DataDir) ->required(),"DataDir,d: string (ex:/data/)")
        ("Generations,g",boost::program_options::value< vector_double >(&Generations) ->multitoken(),"Generations,g: floats (ex:50.2,100.0 )")
        ("InitialTemperature",boost::program_options::value< double >(&InitialTemperature) ,"InitialTemperature: float (ex:120.0 )")
        ("PopulationSize,p",boost::program_options::value< int >(&PopulationSize) ->required(),"PopulationSize,p: int (ex:3500 )")
        ("Nodes",boost::program_options::value< endpoint_list >(&Nodes) ->multitoken()->required(),"Nodes: ip_port_list (ex:192.168.1.3:4900, 192.168.1.4:4900)")
        ("MasterNode",boost::program_options::value< endpoint >(&MasterNode) ->required(),"MasterNode: ip_port (ex:192.168.1.110:4900)")
        ("certificates",boost::program_options::value< url_list >(&certificates) ->multitoken()->required(),"certificates: url_list (ex:http://node.host.net:44434/cert.pem, ftp://node.host.com/cert1.pem)")
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
        if(vars_map["DataDir"].defaulted()||vars_map.count("DataDir"))pVM->insert(std::make_pair("DataDir", ValueType(DataDir)));
        if(vars_map["Generations"].defaulted()||vars_map.count("Generations"))pVM->insert(std::make_pair("Generations", ValueType(Generations)));
        if(vars_map["InitialTemperature"].defaulted()||vars_map.count("InitialTemperature"))pVM->insert(std::make_pair("InitialTemperature", ValueType(InitialTemperature)));
        if(vars_map["PopulationSize"].defaulted()||vars_map.count("PopulationSize"))pVM->insert(std::make_pair("PopulationSize", ValueType(PopulationSize)));
        if(vars_map["Nodes"].defaulted()||vars_map.count("Nodes"))pVM->insert(std::make_pair("Nodes", ValueType(Nodes)));
        if(vars_map["MasterNode"].defaulted()||vars_map.count("MasterNode"))pVM->insert(std::make_pair("MasterNode", ValueType(MasterNode)));
        if(vars_map["certificates"].defaulted()||vars_map.count("certificates"))pVM->insert(std::make_pair("certificates", ValueType(certificates)));
        if(vars_map.count("debug"))pVM->insert(std::make_pair("debug",ValueType(1)));
        else pVM->insert(std::make_pair("debug",ValueType(0)));
        if(vars_map.count("help"))pVM->insert(std::make_pair("help",ValueType(1)));
        else pVM->insert(std::make_pair("help",ValueType(0)));
        return true;
}
