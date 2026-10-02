#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <string>
#include <map>

struct HTTPRequest
{
	std::string method, uri, version;
	std::map<std::string, std::string> headers;
	std::string body;
};

#endif
