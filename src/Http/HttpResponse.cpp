#include "../../inc/Http/HttpResponse.hpp"

std::string	HTTPresponse::toStr()
{
	std::ostringstream str;
	str << "HTTP/1.1 " << _code << " " << reason(_code) << "\r\n";

	str << "host: " << _host << "\r\n";
	for (const auto& par : _headers)
		str << par.first << par.second << "\r\n";
	str << "\r\n";

	if (!_body.empty())
		str << _body;
	return str.str();
}

void		HTTPresponse::setHeader(const std::string& n, const std::string& v)
{
	if (_headers.count(n))
		_headers[n] += ", " + v;
	else
		_headers[n] = v;
}