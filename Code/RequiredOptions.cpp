
#include "RequiredOptions.hpp"

bool RequiredOptions::Parse(std::string const& ConfigName,int argc,char *argv[],boost::shared_ptr<ValueTypeMap> pVM)
{

        boost::program_options::options_description description("Required options");
        path_list  Main_DataDir;
        vector_double  GeneticParameters_Generations;
        double  GeneticParameters_InitialTemperature;
        int  GeneticParameters_PopulationSize;
        std::string  GeneticParameters_CrossoverOp;
        endpoint_list  Cluster_Nodes;
        endpoint  Cluster_MasterNode;
        url_list  Cluster_certificates;
        ;
        description.add_options()
        ("help", "help message")
        ("debug","debug run")
        ("Main.DataDir",boost::program_options::value< path_list >(&Main_DataDir) ->multitoken()->required(),"Main DataDir: path_list (ex:/data/)")
        ("GeneticParameters.Generations",boost::program_options::value< vector_double >(&GeneticParameters_Generations) ->multitoken()->required(),"GeneticParameters Generations: floats (ex:50.2,100.0)")
        ("GeneticParameters.InitialTemperature",boost::program_options::value< double >(&GeneticParameters_InitialTemperature) ->required(),"GeneticParameters InitialTemperature: float (ex:120.0)")
        ("GeneticParameters.PopulationSize",boost::program_options::value< int >(&GeneticParameters_PopulationSize) ->required(),"GeneticParameters PopulationSize: int (ex:3500)")
        ("GeneticParameters.CrossoverOp",boost::program_options::value< std::string >(&GeneticParameters_CrossoverOp) ->required(),"GeneticParameters CrossoverOp: string (ex:G1DListCrossoverTwoPointChunk)")
        ("Cluster.Nodes",boost::program_options::value< endpoint_list >(&Cluster_Nodes) ->multitoken()->required(),"Cluster Nodes: ip_port_list (ex:192.168.1.3:4900,192.168.1.4:4900)")
        ("Cluster.MasterNode",boost::program_options::value< endpoint >(&Cluster_MasterNode) ->required(),"Cluster MasterNode: ip_port (ex:192.168.1.110:4900)")
        ("Cluster.certificates",boost::program_options::value< url_list >(&Cluster_certificates) ->multitoken()->required(),"Cluster certificates: url_list (ex:http://node.host.net:44434/cert.pem,ftp://node.host.com/cert1.pem)")
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
        if(vars_map.count("Main.DataDir"))pVM->insert(std::make_pair("Main.DataDir", ValueType(Main_DataDir)));
        if(vars_map.count("GeneticParameters.Generations"))pVM->insert(std::make_pair("GeneticParameters.Generations", ValueType(GeneticParameters_Generations)));
        if(vars_map.count("GeneticParameters.InitialTemperature"))pVM->insert(std::make_pair("GeneticParameters.InitialTemperature", ValueType(GeneticParameters_InitialTemperature)));
        if(vars_map.count("GeneticParameters.PopulationSize"))pVM->insert(std::make_pair("GeneticParameters.PopulationSize", ValueType(GeneticParameters_PopulationSize)));
        if(vars_map.count("GeneticParameters.CrossoverOp"))pVM->insert(std::make_pair("GeneticParameters.CrossoverOp", ValueType(GeneticParameters_CrossoverOp)));
        if(vars_map.count("Cluster.Nodes"))pVM->insert(std::make_pair("Cluster.Nodes", ValueType(Cluster_Nodes)));
        if(vars_map.count("Cluster.MasterNode"))pVM->insert(std::make_pair("Cluster.MasterNode", ValueType(Cluster_MasterNode)));
        if(vars_map.count("Cluster.certificates"))pVM->insert(std::make_pair("Cluster.certificates", ValueType(Cluster_certificates)));
        if(vars_map.count("debug"))pVM->insert(std::make_pair("debug",ValueType(1)));
        else pVM->insert(std::make_pair("debug",ValueType(0)));
        return true;
}
