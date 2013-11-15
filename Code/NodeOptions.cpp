
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
        ("Main.DataDir",boost::program_options::value< path_list >(&Main_DataDir) ->multitoken()->required(),"Main DataDir: path_list (ex:/data/)")
        ("GeneticParameters.Generations",boost::program_options::value< vector_double >(&GeneticParameters_Generations) ->multitoken(),"GeneticParameters Generations: floats (ex:50.2,100.0 # optional)")
        ("GeneticParameters.InitialTemperature",boost::program_options::value< double >(&GeneticParameters_InitialTemperature) ,"GeneticParameters InitialTemperature: float (ex:120.0 # optional)")
        ("GeneticParameters.PopulationSize",boost::program_options::value< int >(&GeneticParameters_PopulationSize) ->required(),"GeneticParameters PopulationSize: int (ex:3500 # required)")
        ("GeneticParameters.CrossoverOp",boost::program_options::value< std::string >(&GeneticParameters_CrossoverOp) ,"GeneticParameters CrossoverOp: string (ex:G1DListCrossoverTwoPointChunk # optional)")
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
