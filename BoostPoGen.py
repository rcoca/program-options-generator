#!/usr/bin/env python
# razvan.coca@gmail.com
import os,glob,re
import ast
import os, shutil
import sys
sys.path.append('.')
from SectionlessConfigParser import SectionlessConfigParser

from GenHelper import *

def RecurseFindConfigs(directory,glob_pattern):
    CFound=[]
    for root, dirs, files in os.walk(directory):
        allfiles=[os.path.join(root, name) for name in files]
        configs=filter(lambda x:re.search(glob_pattern,x),allfiles)
        CFound.extend(configs)
    return CFound

STypesDict={'ip_port'              : ('endpoint','required'),
            'host_port'            : ('endpoint','required'),
            'path_list'            : ('path_list','multitoken','required'),
            'ip_port_list'         : ('endpoint_list','multitoken','required'),
            'url_list'             : ('url_list','multitoken','required'),
            'url'                  : ('url_string','required'),
            'floats'               : ('vector_double','multitoken','required'),
            'string'               : ('std::string','required'),
            'int'                  : ('int','required'),
            'float'                : ('double','required'),
            'bool'                 : ('bool','required'),
            }

def TypeList():
    return sorted(list(set(map(lambda x:x[0],STypesDict.values()))))


def guessKeyType(text):
    Types=[
        ('([0-9]{1,}\.){3,}[0-9]{1,}[\-:/]\s*[0-9]+\s*,\s*','ip_port_list'),
        ('([0-9]{1,}\.){3,}[0-9]{1,}\s*[\-:/]\s*[0-9]+','ip_port'),
        ('^[A-z\-\.0-9]+[\-:/]\s*[0-9]+$','host_port'),
        ('^\s*([0-9]+\.[0-9]+\s*,\s*)+[0-9]+\.[0-9]+','floats'),
        ('^\s*[0-9]+\.[0-9]+','float'),
        ('^((ftp://|http://|https://|file://).*\s*,)+\s*((ftp://|http://|https://|file://).*)$','url_list'),
        ('^(ftp://|http://|https://|file://).*$','url_string'),
        ('^/[A-z0-9/ \-\.,]+$','path_list'),
        ('^[Tt]rue|[Ff]alse','bool'),
        ('^\s*[0-9]+\s*$','int'),
        ('^[A-z0-9]+.*','string')
        ]
    
    for k,v in Types:
        K=k.replace('$','\s+#\s+.*$')
        if re.search(K,text):
            option=map(lambda x:x.strip(),text.split('#'))
            if len(option)>1:
                return v,[option[-1]]+list(STypesDict[v][1:-1])
            else:
                return v,list(STypesDict[v][1:])
        if re.search(k,text):
            return v,list(STypesDict[v][1:])
 
    return None

def GenTypesAndValidators(typename="ValueType"):
    includes=['#include <string>',
              '#include <iostream>',
              '#include <vector>',
              '#include <boost/variant.hpp>',
              '#include <boost/any.hpp>',
              '#include <boost/program_options.hpp>',
              '#include <boost/regex.hpp>',
              '#include <boost/lexical_cast.hpp>',
              '#include <boost/algorithm/string/split.hpp>',
              '#include <boost/algorithm/string/classification.hpp>']

    CommonIncludes=ForwardDeclsHolder('ConfigCommon',includes,[])
    Classes=[]
    C=ClassHolder('path_list',classtype='struct',includes=['#include <vector>','#include <string>'])
    C.add_member('std::vector<std::string>','paths')
    Classes.append(C)
    C=ClassHolder('endpoint',classtype='struct',includes=['#include <string>'])
    C.add_member('std::string','address')
    C.add_member('unsigned short', 'port')
    Classes.append(C)
    C=ClassHolder('endpoint_list',classtype='struct',includes=['#include <string>','#include <vector>',
                                                               '#include "endpoint.hpp"'])
    C.add_member('std::vector<endpoint>', 'endpoints')
    Classes.append(C)

    C=ClassHolder('url_string',classtype='struct',includes=['#include <string>'])
    C.add_member('std::string','protocol')
    C.add_member('std::string','host')
    C.add_member('std::string', 'path')
    Classes.append(C)

    C=ClassHolder('url_list',classtype='struct',includes=['#include <string>'])
    C.add_member('std::vector<url_string>', 'list')
    Classes.append(C)
    

    C=ClassHolder('vector_double',classtype='struct',includes=['#include <string>','#include <vector>'])
    C.add_member('std::vector<double>', 'values')
    Classes.append(C)

    Functions=FuncCollectionHolder('ConfUtils',includes=includes+\
                                       ['#include "endpoint.hpp"',
                                        '#include "endpoint_list.hpp"',
                                        '#include "path_list.hpp"',
                                        '#include "vector_double.hpp"',
                                        '#include "url_string.hpp"',
                                        '#include "url_list.hpp"'])


    Functions.add_function('std::ostream&', 'operator<<',template_def='template<typename T>',
                           margs=['std::ostream& os', 'const std::vector<T>& v'],
                           body=FileBody('parts/ostream.vectorT.part')())
    

    Functions.add_function('std::ostream&', 'operator<<',
                           margs=['std::ostream& os', 'const vector_double& v'],
                           body=FileBody('parts/ostream.vector_double.part')())
                           

    Functions.add_function('std::ostream &','operator<<',
                           margs=['std::ostream & os','const endpoint& ep'],
                           body=FileBody('parts/ostream.endpoint.part')())


    Functions.add_function('std::ostream &','operator<<',margs=['std::ostream& os','const endpoint_list& elist'],
                           body=FileBody('parts/ostream.endpoints.part')())

    Functions.add_function('std::ostream &','operator<<',
                           margs=['std::ostream & os','const url_string& url'],
                           body=FileBody('parts/ostream.url_string.part')())

    Functions.add_function('std::ostream &','operator<<',
                           margs=['std::ostream & os','const url_list& urls'],
                           body=FileBody('parts/ostream.url_list.part')())

    Functions.add_function('std::ostream &','operator<<',margs=['std::ostream& os','const path_list& plist'],
                           body=FileBody('parts/ostream.path_list.part')())

    Functions.add_function('void','validate',
                           margs=['boost::any& value', 'const std::vector<std::string> & values',
                                  'std::vector<std::string>* target_type', 'int'],
                           body=FileBody('parts/validate.vector_string.part')())

    Functions.add_function('void','validate',
                           margs=['boost::any& value', 'const std::vector<std::string> & values',
                                  'vector_double* target_type', 'int'],
                           body=FileBody('parts/validate.vector_double.part')())

 
    Functions.add_function('void','validate',margs=['boost::any& value',
                                                    'const std::vector<std::string>& values',
                                                    'endpoint* target_type', 'int'],
                           body=FileBody('parts/validate.endpoint.part')())

 
    Functions.add_function('void','validate',margs=['boost::any& value', 
                                                    'const std::vector<std::string>& values', 
                                                    'path_list* target_type','int'],
                           body=FileBody('parts/validate.path_list.part')())


    Functions.add_function('void','validate',margs=['boost::any& value', 
                                                    'const std::vector<std::string>& values',
                                                    'endpoint_list* target_type', 'int'],
                           body=FileBody('parts/validate.endpoint_list.part')())
    
    Functions.add_function('void','validate',margs=['boost::any& value',
                                                    'const std::vector<std::string>& values',
                                                    'url_string* target_type', 'int'],
                           body=FileBody('parts/validate.url_string.part')())

    Functions.add_function('void','validate',margs=['boost::any& value',
                                                    'const std::vector<std::string>& values',
                                                    'url_list* target_type', 'int'],
                           body=FileBody('parts/validate.url_list.part')())

    Classes+=GenInterfaceParser()
    typelist=TypeList()
    Classes.append(GenAccessClass(typename,typelist))
    return [CommonIncludes]+Classes+[Functions]

def GenInterfaceParser(typename="ValueType"):
    Classes=[]
    
    Class=ClassHolder('OptionsParser',classtype='class',includes=[
            '#include <map>','#include <string>',
            '#include <iostream>','#include <vector>',
            '#include <boost/variant.hpp>',
            '#include <boost/shared_ptr.hpp>',
            '#include "endpoint.hpp"',
            '#include "endpoint_list.hpp"','#include "path_list.hpp"',
            '#include "%s.hpp"'%typename],
            typedefs=[(0,GenVariantMapTypeDef(typename=typename))])
    
    Class.add_method('bool','Parse',
                     margs=['std::string const& ConfigName',
                            'int argc','char *argv[]',
                            'boost::shared_ptr<%sMap> pVM'%typename],modifier='virtual',body=0)
    Classes.append(Class)

    return Classes


def GenVariantMapTypeDef(typename='ValueType'):
   return  "typedef std::map<std::string,%s> %sMap;"%(typename,typename)

def GenAccessClass(typename,typelist):
    Class=ClassHolder(typename, includes=['#include <string>','#include <iostream>',
                                          '#include <fstream>',
                                          '#include <vector>',
                                          '#include <boost/variant.hpp>',
                                          '#include "ConfUtils.hpp"',
                                          '#include "endpoint.hpp"',
                                          '#include "endpoint_list.hpp"',
                                          '#include "path_list.hpp"'
                                      ])
    
    Class.add_member("boost::variant<%s >"%(", ".join(TypeList())),'m_data',access='private')
    Class.add_method('',typename,access='private')
    Class.add_method('','~%s'%typename,modifier='virtual',body='')
    for Type in typelist:
        short_type=Type.replace("std::","").replace('<','_').replace('>','')
        Class.add_method('',typename,
                         margs=['%s const & val_%s'%(Type,short_type)],
                         #initializers=["m_data(val_%s)"%short_type],
                         body='    m_data=val_%s;'%short_type)
        Class.add_method(Type,"get_%s"%short_type,body='    return boost::get<%s >(m_data);'%Type)
        Class.add_method('std::ostream &','operator <<',modifier='friend',
                         margs=['std::ostream& os','const %s & T'%typename],
                         body='    os<<T.m_data;\nreturn os;\n')
    return Class

def comp_name(section,option,nosect=False):
    if nosect:return option
    return "_".join([section,option])
def comp_key(section,option,nosect=False):
    if nosect:return option
    return ".".join([section,option])

def GenSpecificParser(ParsedConf,cfg,typename='ValueType'):
    titlename=os.path.basename(cfg).split('.')[0].title()
    includes=['#include "%s.hpp"'%typename, "#include <boost/program_options.hpp>",
              '#include <boost/shared_ptr.hpp>',
              '#include <string>',
              '#include "endpoint.hpp"','#include "path_list.hpp"',
              '#include "endpoint_list.hpp"',
              '#include <cerrno>','#include <cstring>',
              '#include "ConfUtils.hpp"',
              '#include "OptionsParser.hpp"']
    classname="%sOptions"%titlename
    Class=ClassHolder(classname,inherits=['public OptionsParser'],includes=includes,classtype='struct')
    code  = [""]
    code += ["boost::program_options::options_description description(\"%s options\");"%titlename]

    nosection=(len(ParsedConf.sections())==1)
    #print ParsedConf.sections()
    for section in ParsedConf.sections():
        for option in ParsedConf.options(section):
            #print guessKeyType(ParsedConf.get(section,option))
            val_type,option_type=guessKeyType(ParsedConf.get(section,option))
            #print section,option,val_type
            computed_type=STypesDict[val_type][0]
            computed_name=comp_name(section,option,nosect=nosection)
            code  += [("%s  %s;"%(computed_type,computed_name))]
    
    code += [";"]
    code += ["description.add_options()"]
    code += ['("help", "help message")']
    code += ['("debug","debug run")']
    field_template='("%s",boost::program_options::value< %s >(&%s) %s,"%s")'
    print "Summary:"
    for section in ParsedConf.sections():
        for option in ParsedConf.options(section):
            val_type,option_type    = guessKeyType(ParsedConf.get(section,option))
            print "%s\t %s \t (%s)"%(section,option,val_type)
            computed_type=STypesDict[val_type][0]
            computed_name=comp_name(section,option,nosect=nosection)
            #extra_options=["->%s()"%k for k in STypesDict[val_type][1:]]
            option_type=filter(lambda x:x!='optional',option_type)
            extra_options=["->%s()"%k for k in option_type]
            extra_options="".join(extra_options)
            
            computed_kw=comp_key(section,option,nosect=nosection)
            computed_help="%s %s: %s (ex:%s)"%("" if nosection else section,option,val_type,ParsedConf.get(section,option))
            code += [field_template%(computed_kw,computed_type,computed_name,extra_options,computed_help.lstrip())]
            #it could recognize the default section syntax, too, but looks ugly in the help text 
            #if nosection:
            #    computed_kw=comp_key(section,option,nosect=False)
            #    code += [field_template%(computed_kw,computed_type,computed_name,extra_options,computed_help)]
                

    code += [";"]
    code += ['boost::program_options::variables_map vars_map;']
    code += ['boost::program_options::store(boost::program_options::parse_command_line(argc,argv,description),vars_map);']
    code += ['if(vars_map.count("help"))std::cout<<description<<std::endl;']
    code += ['if(vars_map.count("debug"))std::cout<<"debug mode"<<std::endl;']
    code += ['//if config is a usefull parameter in ctor, then use it, else ignore it']
    code += ['if(ConfigName.size())']
    code += ['{']
    code += ['    std::ifstream conf(ConfigName.c_str());']
    code += ['    if(conf.is_open())']
    code += ['        boost::program_options::store(boost::program_options::parse_config_file(conf,description),vars_map);']
    code += ['    else']
    code += ['        throw std::runtime_error(ConfigName+":"+std::strerror( errno ));']
    code += ['    //override config with command line']
    code += ['    boost::program_options::store(boost::program_options::parse_command_line(argc,argv,description),vars_map);']
    code += ['}']
    code += ['boost::program_options::notify(vars_map);']
    
    for section in ParsedConf.sections():
        for option in ParsedConf.options(section):
            val_type,option_type=guessKeyType(ParsedConf.get(section,option))
            #print section,option,val_type
            computed_type=STypesDict[val_type][0]
            computed_key=comp_key(section,option,nosect=nosection)
            computed_name=comp_name(section,option,nosect=nosection)
            code += ['if(vars_map.count("%s"))pVM->insert(std::make_pair("%s", %s(%s)));'%(computed_key,computed_key,typename,computed_name)]
    code += ['if(vars_map.count("debug"))pVM->insert(std::make_pair("debug",%s(1)));'%typename]
    code += ['else pVM->insert(std::make_pair("debug",%s(0)));'%typename]
    code += ['return true;']
    
    Body=("\n"+4*' ').join(code)

    Class.add_method('bool','Parse',margs=['std::string const& ConfigName','int argc','char *argv[]',
                                           'boost::shared_ptr<%sMap> pVM'%typename],body=Body)
    return [Class]

def GenConfigLookupMap(typename='ValueType'):
    includes=['#include <string>','#include <iostream>',
              '#include <boost/thread/mutex.hpp>',
              '#include <boost/shared_ptr.hpp>',
              '#include <boost/program_options.hpp>',
              '#include "ConfUtils.hpp"',
              '#include "OptionsParser.hpp"',
              '#include "%s.hpp"'%typename]
    
    initializers=['FileName(xConfigFile)','m_argc(argc)','m_argv(argv)',
                  'm_map(new %sMap())'%typename]

    Class=ClassHolder('ConfigMap',includes=includes)
    Class.add_method('','~ConfigMap',modifier='virtual',body='')
    Class.add_member('boost::mutex','m_mutex',access='private')
    Class.add_member('boost::shared_ptr<%sMap > '%typename,'m_map',access='private')
    Class.add_member('std::string','FileName',access='private')
    Class.add_member('int','m_argc',access='private')
    Class.add_member('char **','m_argv',access='private')
    Class.add_member('boost::shared_ptr<OptionsParser>',' OptParser',access='private')

    Class.add_method('','ConfigMap',access='private')    
    Class.add_method('','ConfigMap',margs=['OptionsParser *Parser',
                                           'const char *xConfigFile',
                                           'int argc','char *argv[]'],
                     initializers=initializers,body=FileBody('parts/ConfigMap.Ctor.part')())

    Class.add_method('bool', 'ReloadConfig',margs=[],
                     body=FileBody('parts/ConfigMap.Reload.0.part')(typename,typename))

    
    Class.add_method('bool', 'ReloadConfig',margs=['std::string const &xConfigFile'],
                     body=FileBody('parts/ConfigMap.Reload.1.part')(typename,typename))

    Class.add_method(typename,'operator []',margs=['std::string const & key'],
                     body=FileBody('parts/ConfigMap.get.part')(typename))
    Class.add_method(typename,'operator []',margs=['const char* key'],
                     body='    return (*this)[std::string(key)];')
    Class.add_method('std::pair<%sMap::iterator,bool>'%typename,'insert',
                     margs=['std::pair<std::string,%s> val'%typename],
                     body=FileBody('parts/ConfigMap.insert.part')())
    Class.add_method('size_t','erase',margs=['std::string const & key'],
                     body=FileBody('parts/ConfigMap.erase.part')())
    Class.add_method('size_t','size',body='    boost::mutex::scoped_lock lock(m_mutex);\nreturn m_map->size();')
    Class.add_member('typedef','%sMap::iterator iterator'%typename)
    Class.add_member('typedef','%sMap::const_iterator const_iterator'%typename)
    Class.add_method('ConfigMap::iterator','begin',body='   return m_map->begin();')
    Class.add_method('ConfigMap::iterator','end',body='    return m_map->end();')
    Class.add_method(' boost::mutex &','getLock',body='    return m_mutex;',access='public')
    Class.add_method('std::ostream &','operator <<',modifier='friend',
                     margs=['std::ostream& os','ConfigMap & cmap'],
                     body=FileBody('parts/ConfigMap.out.part')())
    
    return [Class]

def writeClassToPath(Class,basedir='.',stream=None):
    if Class.__dict__.has_key('template_class') and  len(Class.template_class):inline=True
    else:inline=False
    if stream==None:
        text=Class.headerBody(inline=inline)
        if text!=None and len(text.strip()):
            with open(os.path.join(basedir,Class.headerName()),"wt+") as stream:
                print >>stream,text
        text=Class.sourceBody(inline=inline)
        if text!=None and len(text)>0:
            with open(os.path.join(basedir,Class.sourceName()),"wt+") as stream:
                print >>stream,text
    else:
        print Class.headerBody(inline=inline)
        if Class.sourceBody(inline=inline)!=None:
            print Class.sourceBody()

def printFilesGeneratedOutput(Items,basedir='Code',stream=None):
    if stream==None:
        try:
            shutil.rmtree(basedir)
        except:pass
        try:
            os.mkdir(basedir)
        except:pass
    for Item in Items:
        writeClassToPath(Item,basedir=basedir,stream=stream)


if __name__ == '__main__':
    if len(sys.argv)>1:bdir=sys.argv[1]
    else: bdir='.'
    Common=GenTypesAndValidators()
    Extra=[]
    for cfg in RecurseFindConfigs(bdir,'.*\.config$'):
        print "\nFound:\t%s\n"%cfg
        #ConfP=ConfigParser.ConfigParser()
        ConfP=SectionlessConfigParser()
        ConfP.optionxform=str
        ConfP.read(cfg)
        Extra+=GenSpecificParser(ConfP,cfg,typename='ValueType')
    LockedConf=GenConfigLookupMap(typename='ValueType')
    printFilesGeneratedOutput(Common+Extra+LockedConf,basedir=os.path.join(bdir,'ConfigCode'))
    #printFilesGeneratedOutput(Common+Extra+LockedConf,stream=sys.stdout)
        
