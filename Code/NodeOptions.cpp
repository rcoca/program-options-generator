
#include "NodeOptions.hpp"

bool NodeOptions::Parse(std::string const& ConfigName,int argc,char *argv[],boost::shared_ptr<ValueTypeMap> pVM)
{

        boost::program_options::options_description description("Node options");
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
        ("Main.DataDir",boost::program_options::value< path_list >(&Main_DataDir) ->multitoken()->required(),"Main DataDir: path_list")
        ("GeneticParameters.Generations",boost::program_options::value< vector_double >(&GeneticParameters_Generations) ->multitoken()->required(),"GeneticParameters Generations: floats")
        ("GeneticParameters.InitialTemperature",boost::program_options::value< double >(&GeneticParameters_InitialTemperature) ->required(),"GeneticParameters InitialTemperature: float")
        ("GeneticParameters.PopulationSize",boost::program_options::value< int >(&GeneticParameters_PopulationSize) ->required(),"GeneticParameters PopulationSize: int")
        ("GeneticParameters.CrossoverOp",boost::program_options::value< std::string >(&GeneticParameters_CrossoverOp) ->required(),"GeneticParameters CrossoverOp: string")
        ("Cluster.Nodes",boost::program_options::value< endpoint_list >(&Cluster_Nodes) ->multitoken()->required(),"Cluster Nodes: ip_port_list")
        ("Cluster.MasterNode",boost::program_options::value< endpoint >(&Cluster_MasterNode) ->required(),"Cluster MasterNode: ip_port")
        ("Cluster.certificates",boost::program_options::value< url_list >(&Cluster_certificates) ->multitoken()->required(),"Cluster certificates: url_list")
        ;
        boost::program_options::variables_map vars_map;
        std::ifstream conf(ConfigName.c_str());
        boost::program_options::store(boost::program_options::parse_command_line(argc,argv,description),vars_map);
        if(vars_map.count("help"))std::cout<<description<<std::endl;
        if(vars_map.count("debug"))std::cout<<"debug mode"<<std::endl;
        if(conf.is_open())
            boost::program_options::store(boost::program_options::parse_config_file(conf,description),vars_map);
        else
           throw std::runtime_error(ConfigName+":"+std::strerror( errno ));
        boost::program_options::store(boost::program_options::parse_command_line(argc,argv,description),vars_map);
        boost::program_options::notify(vars_map);
        pVM->insert(std::make_pair("Main.DataDir", ValueType(Main_DataDir)));
        pVM->insert(std::make_pair("GeneticParameters.Generations", ValueType(GeneticParameters_Generations)));
        pVM->insert(std::make_pair("GeneticParameters.InitialTemperature", ValueType(GeneticParameters_InitialTemperature)));
        pVM->insert(std::make_pair("GeneticParameters.PopulationSize", ValueType(GeneticParameters_PopulationSize)));
        pVM->insert(std::make_pair("GeneticParameters.CrossoverOp", ValueType(GeneticParameters_CrossoverOp)));
        pVM->insert(std::make_pair("Cluster.Nodes", ValueType(Cluster_Nodes)));
        pVM->insert(std::make_pair("Cluster.MasterNode", ValueType(Cluster_MasterNode)));
        pVM->insert(std::make_pair("Cluster.certificates", ValueType(Cluster_certificates)));
        if(vars_map.count("debug"))pVM->insert(std::make_pair("debug",ValueType(1)));
        else pVM->insert(std::make_pair("debug",ValueType(0)));
        return true;
}
