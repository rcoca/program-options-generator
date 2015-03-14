# Introduction #

Python script uses Config parser to guess variable types and then produces c++ code that parses that config files. C++ code is based on boost::program\_options.

Parser is used in such a way that command line arguments override config and there are two default options -debug and help- that allow a debug run respectively a short help page.

Result is stored in a std::map slightly wrapped in another class equipped with mutex access for "[ ]" operator.

Support for reload is provided by reparsing the new config and swapping pointers while mutex is locked. If it fails, retains the old config.
Iterator access is not locked, a getLock is provided for the user to do so if he so wishes.

```
[Main]
DataDir = /data/

[GeneticParameters]
Generations=50
InitialTemperature=120.0
PopulationSize=3500
CrossoverOp=G1DListCrossoverTwoPointChunk

[Cluster]
Nodes=192.168.1.3:4900,192.168.1.4:4900
MasterNode=192.168.1.110:4900

```

produces something like:
```
        description.add_options()
        ("help", "help message")
        ("debug","debug run")
        ("Main.DataDir",boost::program_options::value< path_list >(&Main_DataDir) ->multitoken()->required(),"Main DataDir: path_list")
        ("GeneticParameters.Generations",boost::program_options::value< int >(&GeneticParameters_Generations) ->required(),"GeneticParameters Generations: int")
        ("GeneticParameters.InitialTemperature",boost::program_options::value< double >(&GeneticParameters_InitialTemperature) ->required(),"GeneticParameters InitialTemperature: float")
        ("GeneticParameters.PopulationSize",boost::program_options::value< int >(&GeneticParameters_PopulationSize) ->required(),"GeneticParameters PopulationSize: int")
        ("GeneticParameters.CrossoverOp",boost::program_options::value< std::string >(&GeneticParameters_CrossoverOp) ->required(),"GeneticParameters CrossoverOp: string")
        ("Cluster.Nodes",boost::program_options::value< endpoint_list >(&Cluster_Nodes) ->multitoken()->required(),"Cluster Nodes: ip_port_list")
        ("Cluster.MasterNode",boost::program_options::value< endpoint >(&Cluster_MasterNode) ->required(),"Cluster MasterNode: ip_port")
        ;

```

---

# Usage #
**Basic:**

just run it in the directory that contains and ini file. It will generate common code and specific code for that ini file.

Two extra flags are inserted: debug and help that allow a debug run and printing help. Help content is extracted from the contents of ini file.

**If ini file contains:**

```
key1 = value1
key2  = value # optional 
key3 = value # default_value(3)
```
the generator will generate a required field for key1,an optional field for key2 and an optional key3 with value defaulted to 3.

Lack of section yields a flat parser, without need for sections.

**Instantiation of OptionsParser**

if it gets an empty string, config file isn't looked for, instead, only the command line is used. This behavior can be changed at instantiation, just give it a path to a file.