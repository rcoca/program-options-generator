
#include "ValueType.hpp"

ValueType::ValueType(bool const & val_bool)
{
    m_data=val_bool;
}
ValueType::ValueType(double const & val_double)
{
    m_data=val_double;
}
ValueType::ValueType(endpoint const & val_endpoint)
{
    m_data=val_endpoint;
}
ValueType::ValueType(endpoint_list const & val_endpoint_list)
{
    m_data=val_endpoint_list;
}
ValueType::ValueType(int const & val_int)
{
    m_data=val_int;
}
ValueType::ValueType(path_list const & val_path_list)
{
    m_data=val_path_list;
}
ValueType::ValueType(std::string const & val_string)
{
    m_data=val_string;
}
bool ValueType::get_bool()
{
    return boost::get<bool >(m_data);
}
double ValueType::get_double()
{
    return boost::get<double >(m_data);
}
endpoint ValueType::get_endpoint()
{
    return boost::get<endpoint >(m_data);
}
endpoint_list ValueType::get_endpoint_list()
{
    return boost::get<endpoint_list >(m_data);
}
int ValueType::get_int()
{
    return boost::get<int >(m_data);
}
path_list ValueType::get_path_list()
{
    return boost::get<path_list >(m_data);
}
std::string ValueType::get_string()
{
    return boost::get<std::string >(m_data);
}
std::ostream & operator <<(std::ostream& os,const ValueType & T)
{
    os<<T.m_data;
    return os;
}
ValueType::~ValueType()
{

}
