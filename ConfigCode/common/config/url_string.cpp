
#include "url_string.hpp"

url_string::url_string()
{

}
url_string::url_string(std::string const& proto,std::string const& host_,std::string const&  path_)
    :protocol(proto), host(host_), path(path_)
{

}
