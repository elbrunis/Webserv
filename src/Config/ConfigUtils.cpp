#include "../../inc/Config/ConfigParser.hpp"
#include "../../inc/Headers.hpp"

const std::string&	ConfigParser::nextToken()
{
	if (_index >= _tokens.size())
		throw std::runtime_error("config: fin de archivo inesperado");
	return _tokens[_index++];
}

const std::string&	ConfigParser::peekToken()
{
	if (_index >= _tokens.size())
		throw std::runtime_error("config: fin de archivo inesperado");
	return _tokens[_index];
}

bool	ConfigParser::expect(std::string s)
{
	if (_index >= _tokens.size() || _tokens[_index] != s)
		return false;
	return true;
}

std::vector<std::string>	ConfigParser::parseArgs()
{
	std::vector<std::string> args;

	while (_index < _tokens.size() && peekToken() != ";")
	{
		if (peekToken() == "{" || peekToken() == "}")
			throw std::runtime_error("args: unespected char '{' or '}'");
		args.push_back(peekToken());
		nextToken();
	}
	if(!expect(";"))
		throw std::runtime_error("args: expected ';' after args");
	nextToken();
	return args;
}

bool	ConfigParser::hasExactSize(const std::vector<std::string>& v, size_t n)
{
	return v.size() == n;
}

size_t	ConfigParser::toNum(const std::string& s)
{
	if (s.empty() || s.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("config: número inválido '" + s + "'");
	return std::strtoul(s.c_str(), NULL, 10);
}

void ConfigParser::validate() const
{
	if (_index != _tokens.size())
		throw std::runtime_error("server: error al buscar server");
	if (_servers.empty())
		throw std::runtime_error("config: no hay ningún server");

	for (size_t i = 0; i < _servers.size(); ++i)
	{
		const ServerConfig& s = _servers[i];
		for (size_t a = 0; a < s.locations.size(); ++a)
			for (size_t b = a + 1; b < s.locations.size(); ++b)
				if (s.locations[a].path == s.locations[b].path)
					throw std::runtime_error("config: location duplicada '" + s.locations[a].path + "'");
	}
}
