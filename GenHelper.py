#razvan.coca@gmail.com
import re
from collections import OrderedDict
import sys,os

class FileBody:
    def __init__(self,filename):
        def getFile(filename):
            with open(filename,'r') as f:
                snippet=f.read()
            return snippet
        self.snippet=""
        try:
            self.snippet=getFile(filename)
        except IOError,e:
            newfilename=os.path.join(os.path.dirname(__file__),filename)
            #print "rerouting path to %s"%newfilename
            self.snippet=getFile(newfilename)
    def __call__(self,*args):
        return self.snippet%args

class ClassHolder:
    """
    tries to make a place to keep a class content.
    features header generation/inline or not, template 
    allowing 1 file for all and reorganizing based on multiplefiles
    source file includes the header file. support for separate source file includes
    """
    def __init__(self,name,inherits=[],includes=[],
                 classtype='class',template_class='',srcincludes=[],typedefs=[]):
        self.name=name #string

        self.inheritsfrom=inherits #list of inheritances
        self.datamembers=OrderedDict() #
        self.methods={}
        self.template_methods={}
        self.includes=includes
        self.classtype=classtype
        self.template_class=template_class
        self.srcincludes=srcincludes
        self.typedefs=typedefs
    def add_member(self,mtype,mname,access='public'):
        self.datamembers[mname]={'type':mtype,'access':access,'name':mname}
    def add_method(self,mtype,mname,margs=[],access='public',modifier='',body=None,initializers=None,qualifier=None):
        k='%s.%s'%(mname,".".join(margs))
        self.methods[k]={'type':mtype,'name':mname,'modifier':modifier,
                             'args':margs,'access':access,'body':body,
                             'ilist':initializers,'qualifier':qualifier}
    def add_templated_method(self,mtemplate_params,mtype,mname,margs=[],access='public',modifier='',body='{};'):
        k="%s.%s.%s"%(".".join(mtemplate_params),mname,".".join(margs))
        self.template_methods[k]={'params':mtemplate_params,'name':mname,
                                      'type':mtype,
                                      'args':margs,'access':access,
                                      'body':body}
    def headerName(self):
        return "%s.hpp"%self.name
    def sourceName(self):
        return "%s.cpp"%self.name
    def memberDecl(self,m):
        return "    %s %s;\n"%(self.datamembers[m]['type'],self.datamembers[m]['name'])
    def methodDecl(self,m,inline=True):
        if self.methods[m]['ilist'] and inline:
            initializers=":    "+", ".join(self.methods[m]['ilist'])
        else:
            initializers=' '+(self.methods[m]['qualifier'] if self.methods[m]['qualifier'] else '')
        if inline and self.methods[m]['body']!=None and self.methods[m]['body']!=0:
            insertBody="{%s}"%self.methods[m]['body']
        elif self.methods[m]['body']==0:
            insertBody="    = 0"
        else: 
            insertBody=""
        return "    "+("%s%s%s(%s)%s  %s;\n"%(self.methods[m]['modifier']+' ',
                                           self.methods[m]['type']+' ',
                                           self.methods[m]['name']+' ',
                                           ", ".join(self.methods[m]['args']),
                                           initializers,insertBody)).lstrip()
    def mtemplateDecl(self,m,inline=True):

        templateargs=["typename %s"%t for t in self.template_methods[m]['params']]            
        code  = [""]
        code += ["template<%s >\n"%(", ".join(templateargs))]
        code += ["    %s %s(%s)"%(self.template_methods[m]['type'],
                                     self.template_methods[m]['name'],
                                     ", ".join(self.template_methods[m]['args']))]
        code += ["{%s};\n"%self.template_methods[m]['body']]
        return " ".join(code)

    def headerBody(self,inline=False):
        headerdefine=self.headerName().replace('.','_')
        doubleinclude="#ifndef %s\n#define %s\n%%s\n#endif /*#ifdef %s*/"%((headerdefine,)*3)
        inheritDecl='' if len(self.inheritsfrom)==0 else ":"+", ".join(self.inheritsfrom)
        code=[""]
        classScope="\n".join(self.includes)+"\n"*2
        classScope +="\n"+"\n".join(map(lambda z:z[1],filter(lambda x:x[0]==0,self.typedefs)))+"\n"
        classScope += "%s\n%s %s %s\n{%%s};"%(self.template_class,self.classtype,self.name,inheritDecl)
        

        for accesstype in ["private","protected","public"]:
            pmembers=filter(lambda x:self.datamembers[x]['access']==accesstype,self.datamembers)
            pmethods=filter(lambda x:self.methods[x]['access']==accesstype,self.methods)
            ptemplates=filter(lambda x:self.template_methods[x]['access']==accesstype,self.template_methods)
            if len(pmembers)+len(pmethods)+len(ptemplates)>0:
                code+=["%s:"%accesstype]
                for m in pmembers:   code+=[self.memberDecl(m)]
                for m in sorted(pmethods):   code+=[self.methodDecl(m,inline=inline)]
                for m in sorted(ptemplates): code+=[self.mtemplateDecl(m,inline=inline)]
        classDecl  = classScope%"\n".join(code)
        classDecl += "\n".join(map(lambda z:z[1],filter(lambda x:x[0]==1,self.typedefs)))+"\n"
        return doubleinclude%classDecl

    def sourceBody(self,inline=False):
        if inline:return None
        code  = [""]
        code += ['#include "%s"'%self.headerName()]
        code += self.srcincludes
        code += [""]
        method_found=False
        for method in sorted(self.methods):
            if type(self.methods[method]['body'])==type(""):
                method_found=True
                if self.methods[method]['ilist']:
                    initializers=":"+", ".join(self.methods[method]['ilist'])
                else:
                    initializers=' '+(self.methods[method]['qualifier'] if self.methods[method]['qualifier'] else '')
                    #initializers=''
                args=map(lambda x:re.sub('=.*$','',x),self.methods[method]['args'])
                #args=map(lambda x:re.sub('=.*[,\)]','',x),self.methods[method]['args'])
                if self.methods[method]['modifier']!='friend':
                    code += [("%s %s::%s(%s)%s"%(self.methods[method]['type'].strip(),
                                                 self.name.strip(),self.methods[method]['name'].strip(),
                                                 ",".join(args).strip(),
                                                 "" if len(initializers)==0 else '\n'+4*' '+initializers))]
                else:
                    code += [("%s %s(%s)%s"%(self.methods[method]['type'].strip(),
                                             self.methods[method]['name'].strip(),
                                             ",".join(args).strip(),
                                             "" if len(initializers)==0 else '\n'+4*' '+initializers))]
                    
                #print code
                code[-1]=code[-1].lstrip()
                code += ["{\n%s\n}"%("\n    ".join(self.methods[method]['body'].splitlines()))]
        Body="\n".join(code)
        if len(Body.strip())==0:return None
        if method_found: return Body
        return None

class FuncCollectionHolder:
    def __init__(self,name,includes=[],srcincludes=[]):
        self.name=name
        self.includes=includes
        self.srcincludes=srcincludes
        self.methods=OrderedDict()

    def headerName(self):
        return "%s.hpp"%self.name
    def sourceName(self):
        return "%s.cpp"%self.name

    def add_function(self,mtype,mname,margs=[],modifier='',body=None,template_def=''):

        k='%s.%s'%(mname,".".join(margs))
        self.methods[k]={'type':mtype,'name':mname,'modifier':modifier,
                             'args':margs,'body':body,'template':template_def}
        
    def headerBody(self,inline=False):
        headerdefine=self.headerName().replace('.','_')
        doubleinclude="#ifndef %s\n#define %s\n%%s\n#endif /*#ifdef %s*/"%((headerdefine,)*3)

        code=["\n"]
        Scope="\n".join(self.includes)+"\n"*4
        for m in self.methods:
            code+=[self.methodDecl(m,inline=inline)]
        Decl=Scope%"\n".join(code)
        return doubleinclude%Decl
        
    def methodDecl(self,m,inline=False):
        if (inline or len(self.methods[m]['template'])) and self.methods[m]['body']!=None:
            insertBody="\n{%s\n}\n"%self.methods[m]['body']
        else: 
            insertBody=";"
        args  = self.methods[m]['args']
        code  = ("%s\n"%self.methods[m]['template']).lstrip()
        code += ("%s%s%s(%s)"%(self.methods[m]['modifier']+' ',
                                 self.methods[m]['type']+' ',
                                 self.methods[m]['name']+' ',
                                 ", ".join(args))).lstrip()
        code += insertBody
        return code

    def methodDef(self,m):
        if len(self.methods[m]['template']):
            return ""
        elif self.methods[m]['body']!=None: 
            insertBody="{\n%s\n}"%self.methods[m]['body']
        else: insertBody=""

        args=map(lambda x:re.sub('=.*[,\)]','',x),self.methods[m]['args'])
        code  =("%s\n"%self.methods[m]['template']).lstrip()
        code += ("%s%s%s(%s)\n"%(self.methods[m]['modifier']+' ',
                                 self.methods[m]['type']+' ',
                                 self.methods[m]['name']+' ',
                                 ", ".join(args))).lstrip()
        code += insertBody
        return code
        
    def headerBody(self,inline=False):
        headerdefine=self.headerName().replace('.','_')
        doubleinclude="#ifndef %s\n#define %s\n%%s\n#endif /*#ifdef %s*/"%((headerdefine,)*3)

        code=[""]
        funcScope="\n".join(self.includes)+"\n"*4+"%s\n"
        for m in self.methods:   code+=[self.methodDecl(m,inline=inline)]
        Decls=funcScope%("\n".join(code))
        return doubleinclude%Decls

    def sourceBody(self,inline=False):
        if inline:return None
        code  = [""]
        code += ['#include "%s"'%self.headerName()]
        code += self.srcincludes
        code += [""]
        method_found=False
        for method in self.methods:
            if type(self.methods[method]['body'])==type(""):
                method_found=True
                code += [self.methodDef(method)]
        if method_found:
            return "\n".join(code)
        return None
    
    
class ForwardDeclsHolder:
    """
    Used in case we just need a place to do:
    template instatiation
    include bunch
    forward declarations
    typedefs
    """

    def __init__(self,name,includes,decl_list):
        self.name=name
        self.includes=includes
        self.decl_list=decl_list
    def headerName(self):
        return "%s.hpp"%self.name
    def sourceName(self):
        return "%s.cpp"%self.name

    def Body(self):
        code  = ['']
        code += self.includes
        code += ['']
        code += self.decl_list
        return "\n".join(code)
    def sourceBody(self,inline=False):
        return None

    def headerBody(self,inline=False):
        headerdefine=self.headerName().replace('.','_')
        doubleinclude="#ifndef %s\n#define %s\n %%s\n#endif /*#ifdef %s*/"%((headerdefine,)*3)
        return doubleinclude%self.Body()

def GenerateDotNode(Class):
    for accesstype in ['public']:
        pmethods=filter(lambda x:Class.methods[x]['access']==accesstype,Class.methods)
        ptemplates=filter(lambda x:Class.template_methods[x]['access']==accesstype,Class.template_methods)

    dmethods=["+%s()"%Class.methods[m]['name'] for m in pmethods]
    tmethods=["+%s<%s>()"%(Class.template_methods[m]['name'],",".join(Class.template_methods[m]['params'])) for m in ptemplates]
    return "%s[%s];"%(Class.name,'shape=record,label="'+('{%s||%s|%s}"'%(Class.name,"|".join(dmethods),"|".join(tmethods))))

def GenerateDotEdgeList(List):
    edges=[""]
    for s in List:
        for i in s.inheritsfrom:
            true_i=i.replace('virtual','').replace('public','').strip()
            e='"%s"->"%s"[arrowhead=empty];'%(s.name,true_i)
            if not e in edges:
                edges += [e]
    for s in List:
        smembers=map(lambda x:x['type'],s.datamembers.values())
        for d in List:
            dmembers=map(lambda x:x['type'],d.datamembers.values())
            for dm in dmembers:
                flags=map(lambda x:re.search(x,dm),
                          ['bool','int','std::string','using', 'typedef','unsigned','enum','boost::function','boost::asio','boost::posix_time'])
                if any(flags):continue
                flags=map(lambda x:re.search(x,s.name),
                          ['bool','int','std::string','using', 'typedef','unsigned','enum','boost::function','boost::asio','boost::posix_time'])
                if any(flags):continue
                if dm in smembers:
                    e=['"%s"->"%s"[arrowhead=diamond];'%(dm,s.name)]
                elif ("boost::shared_ptr<%s>"%dm) in smembers:
                    e=['"%s"->"%s"[arrowhead=diamond,color=grey];'%(dm,s.name)]
                if not e in edges:edges+=e
                
    return ("\n"+4*' ').join(edges)

def GenerateDotFromClassList(List):
    graph="""
digraph G{
        rankdir = BT;
        fontname = "Bitstream Vera Sans"
        fontsize = 8
        //nodesep= 0.6
        splines=ortho
        node [fontname = "Bitstream Vera Sans",fontsize = 8,color="#BBBBBB",fillcolor="#8DAAEA",
             style=filled,shape=record];
        edge [fontname = "Bitstream Vera Sans",fontsize = 10 ];
        %s
        %s
}
"""
    dClasses=[]
    for Class in List:
        dClasses+=[GenerateDotNode(Class)]
    dEdges=GenerateDotEdgeList(List)
    dot_string=graph%("\n    ".join(dClasses),dEdges)
    return dot_string

