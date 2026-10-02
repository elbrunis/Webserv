#include "../../inc/Http/HttpResponse.hpp"

std::string	HTTPResponse::toStr()
{
	std::ostringstream str;
	str << "HTTP/1.1 " << _code << " " << reason(_code) << "\r\n";

	str << "host: " << _host << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		str << it->first << it->second << "\r\n";
	str << "\r\n";

	if (!_body.empty())
		str << _body;
	return str.str();
}

void		HTTPResponse::setHeader(const std::string& n, const std::string& v)
{
	if (_headers.count(n))
		_headers[n] += ", " + v;
	else
		_headers[n] = v;
}