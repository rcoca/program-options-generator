
#include "ConfUtils.hpp"


std::ostream& operator<< (std::ostream& os, const vector_double& v)
{

    typename std::vector<double>::const_iterator i=v.values.begin();
    os<<"[ ";
    for(;i!=v.values.end();++i)
        os<<*i<<(i+1==v.values.end()?" ]":", ");
    return os;
}
std::ostream & operator<< (std::ostream & os, const endpoint& ep)
{

    os<<"["<<ep.address<<"]:["<<ep.port<<"]";
    return os;
}
std::ostream & operator<< (std::ostream& os, const endpoint_list& elist)
{

    os<<elist.endpoints;
    return os;
}
std::ostream & operator<< (std::ostream & os, const url_string& url)
{

    os<<url.protocol<<"://"<<url.host<<"/"<<url.path;
    return os;
}
std::ostream & operator<< (std::ostream & os, const url_list& urls)
{

    os<<urls.list;
    return os;
}
std::ostream & operator<< (std::ostream& os, const path_list& plist)
{

    os<<plist.paths;
    return os;
}
void validate (boost::any& value, const std::vector<std::string> & values, std::vector<std::string>* target_type, int)
{

    boost::program_options::validators::check_first_occurrence(value);
    const std::string& s = boost::program_options::validators::get_single_string(values);
    std::vector< std::string > tokens;
    boost::split( tokens, s, boost::is_any_of( "," ) );
    value=boost::any(tokens);
}
void validate (boost::any& value, const std::vector<std::string> & values, vector_double* target_type, int)
{

    boost::program_options::validators::check_first_occurrence(value);
    const std::string& s = boost::program_options::validators::get_single_string(values);
    std::vector< std::string > tokens;
    vector_double  numbers;
    boost::split( tokens, s, boost::is_any_of( "," ) );
    std::vector<std::string>::iterator i=tokens.begin();
    for(;i!=tokens.end();i++)
    {
        double number;
        try
        {
            number=boost::lexical_cast<double,std::string>(*i);
            numbers.values.push_back(number);
        }
        catch(boost::bad_lexical_cast &e)
        {
            throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value,"double",s);
        }
    }
    value=boost::any(numbers) ;
}
void validate (boost::any& value, const std::vector<std::string>& values, endpoint* target_type, int)
{

    endpoint Endpoint;
    boost::program_options::validators::check_first_occurrence(value);
    const std::string& String = boost::program_options::validators::get_single_string(values);
    std::vector< std::string > tokens;
    boost::split( tokens, String, boost::is_any_of( "/:-" ) );
    if ( tokens.size()!=2 )
    throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value,"endpoint",String );
    Endpoint.address=tokens[0];
    try
    {
        Endpoint.port=boost::lexical_cast<unsigned short,std::string>(tokens[1]);
    }
    catch(boost::bad_lexical_cast &e)
    {
        throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value,"endpoint",String);
    }
    value=boost::any(Endpoint);
}
void validate (boost::any& value, const std::vector<std::string>& values, path_list* target_type, int)
{

    path_list paths;
    boost::program_options::validators::check_first_occurrence(value);
    const std::string& str = boost::program_options::validators::get_single_string(values);
    std::vector< std::string > tokens;
    boost::split( paths.paths, str, boost::is_any_of( "," ) );
    value=boost::any(paths);
}
void validate (boost::any& value, const std::vector<std::string>& values, endpoint_list* target_type, int)
{

    std::vector<std::string> list_elements;
    endpoint_list end_points;
    boost::program_options::validators::check_first_occurrence(value);
    const std::string& str = boost::program_options::validators::get_single_string(values);
    boost::split( list_elements , str, boost::is_any_of( "," ) );
    std::vector<std::string>::iterator i=list_elements.begin();
    for(;i!=list_elements.end();i++)
    {
        endpoint Endpoint;
        std::vector< std::string > tokens;
        boost::split( tokens, *i, boost::is_any_of( "/:-" ) );
        if (tokens.size() !=2)
             throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value,  "endpoint",str );
        Endpoint.address=tokens[0];
        try
        {
              Endpoint.port=boost::lexical_cast<unsigned short,std::string>(tokens[1]);
        }
        catch(boost::bad_lexical_cast &e)
        {
            throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value,"endpoint",str);
        }
        end_points.endpoints.push_back(Endpoint);
    }
    value=boost::any(end_points);
}
void validate (boost::any& value, const std::vector<std::string>& values, url_string* target_type, int)
{
    url_string url;
    const std::string& input = boost::program_options::validators::get_single_string(values);


    boost::regex exrp( "^([A-z]+)\\://([A-z\\.\\-]+(\\:[0-9]+)*)[/]?(.*)$") ;
    boost::match_results<std::string::const_iterator> what;
    if( regex_search( input, what, exrp ) )
    {
        url.protocol=std::string( what[1].first, what[1].second );
        url.host=std::string( what[2].first, what[2].second );
        url.path=std::string( what[what.size()-1].first, what[what.size()-1].second );
    }
    else
    {
        throw boost::program_options::validation_error(
            boost::program_options::validation_error::invalid_option_value,
            "url_string",
            input);
    }
    value=boost::any(url);

}
void validate (boost::any& value, const std::vector<std::string>& values, url_list* target_type, int)
{

    std::vector<std::string> list_elements;
    url_list urls;
    boost::program_options::validators::check_first_occurrence(value);
    const std::string& s = boost::program_options::validators::get_single_string(values);
    boost::split( list_elements , s, boost::is_any_of( "," ) );
    std::vector<std::string>::iterator i=list_elements.begin();
    boost::regex exrp( "^([A-z]+)\\://([A-z\\.\\-]+(\\:[0-9]+)*)[/]?(.*)$") ;
    for(; i!=list_elements.end(); i++)
    {
        url_string url;
        boost::match_results<std::string::const_iterator> what;

        if( regex_search( *i, what, exrp ) )
        {
            url.protocol=std::string( what[1].first, what[1].second );
            url.host=std::string( what[2].first, what[2].second );
            url.path=std::string( what[what.size()-1].first, what[what.size()-1].second );
            urls.list.push_back(url);
        }
        else
        {
            throw boost::program_options::validation_error(
                boost::program_options::validation_error::invalid_option_value,
                "url_list",
                *i);
        }
    }

    value=boost::any(urls);

}
