
#include "ConfUtils.hpp"


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
    value=boost::any(tokens) ;
}
void validate (boost::any& value, const std::vector<std::string> & values, std::vector<double>* target_type, int)
{

    boost::program_options::validators::check_first_occurrence(value);
    const std::string& s = boost::program_options::validators::get_single_string(values);
    std::vector< std::string > tokens;
    std::vector< double > numbers;
    boost::split( tokens, s, boost::is_any_of( "," ) );
    std::vector<std::string>::iterator i=tokens.begin();
    for(;i!=tokens.end();i++)
    {
        double number;
        try
        {
            number=boost::lexical_cast<double,std::string>(*i);
            numbers.push_back(number);
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
