#include "../../inc/Config/ConfigParser.hpp"
#include "../../inc/Headers.hpp"

LocationConfig	ConfigParser::parseLocation()
{
    LocationConfig l;
    l.path = nextToken();
    if (l.path[0] != '/')
		throw std::runtime_error("config: location debe empezar por '/'");
	if (!expect("{"))
		throw std::runtime_error("location: expected {");
	nextToken();
	while (true)
	{
		std::string key = nextToken();
		if (key == "}") 
			break;
		std::vector<std::string> a = parseArgs();
		if (key == "root" && a.size() == 1)
			l.root = a[0];
		else if (key == "index" && a.size() == 1)
			l.index = a[0];
		else if (key == "autoindex" && a.size() == 1)
			l.autoindex = (a[0] == "on");
		else if (key == "upload_store" && a.size() == 1)
			l.uploadStore = a[0];
		else if (key == "cgi" && a.size() == 2)
			l.cgi[a[0]] = a[1];
		else if (key == "client_max_body_size" && a.size() == 1)
			{ l.maxBody = toNum(a[0]); l.hasMaxBody = true; }
		else if (key == "return" && a.size() == 2)
			{ l.redirectCode = toNum(a[0]); l.redirectUrl = a[1]; }
		else if (key == "methods" && !a.empty())
		{
			for (size_t i = 0; i < a.size(); ++i) {
				if (a[i] != "GET" && a[i] != "POST" && a[i] != "DELETE")
					throw std::runtime_error("config: método inválido '" + a[i] + "'");
				l.methods.insert(a[i]);
			}
		}
		else throw std::runtime_error("config: directiva inválida '" + key + "'");
	}
	return l;
}

bool	ConfigParser::parseServerLoop(ServerConfig& s, std::string& key, std::vector<std::string>& a)
{
	key.clear();
	a.clear();
	if (expect("}"))
		{nextToken(); return false;}
	if (peekToken() == "location")
	{
		nextToken();
		s.locations.push_back(parseLocation());
		return true;
	}
	key = nextToken(); // devuelve el actual y avanza tkn++
	a = parseArgs();
	if (key == "server_name" && !a.empty())
		s.serverNames = a;
	else if (key == "root" && a.size() == 1)
		s.root = a[0];
	else if (key == "index" && a.size() == 1)
		s.index = a[0];
	else if (key == "client_max_body_size" && a.size() == 1)
		s.maxBody = toNum(a[0]);
	else if (key == "error_page" && a.size() == 2)
		s.errorPages[toNum(a[0])] = a[1];
	else
		throw std::runtime_error("config: directiva inválida '" + key + "'");
	return true;
}
void ConfigParser::applyInheritance(ServerConfig& s)
{
	for (size_t i = 0; i < s.locations.size(); ++i)
	{
		LocationConfig& l = s.locations[i];
		if (l.root.empty())             l.root = s.root;
		if (l.index.empty())            l.index = s.index;
		if (!l.hasMaxBody)              l.maxBody = s.maxBody;
		if (l.methods.empty())          l.methods.insert("GET");
	}
}

ServerConfig	ConfigParser::parseServer() // por algun motivo me da error el nombre  si le quito el &
{
	ServerConfig				s;
	std::string					key;
	std::vector<std::string>	args;

	if (!expect("{"))
		throw std::runtime_error("server: expected {");
	nextToken();

	// parse port
	key = nextToken(); // devuelve el actual y avanza tkn++
	if (key != "listen")
		throw std::runtime_error("server: expected listen");	
	args = parseArgs();
	if (!hasExactSize(args, 1))
		throw std::runtime_error("listen: unexpected port");
	size_t	has = args[0].find(":");
	if (has != std::string::npos)
		s.host = args[0].substr(0, has);
	s.port = toNum(args[0].substr(has == std::string::npos ? 0 : has + 1));
    if (s.port < 1 || s.port > 65535)
		throw std::runtime_error("config: puerto inválido");

	// parse rest
	bool next = true;
	while (next)
		next = parseServerLoop(s, key, args);
	applyInheritance(s);
	return s;
}

std::vector<ServerConfig>	ConfigParser::parse(const std::string& filepath)
{
	tokenize(filepath);
	while (_index < _tokens.size())
	{
		if (peekToken() != "server")
			break;
		nextToken();
		ServerConfig server;
		server = parseServer();
		_servers.push_back(server);
	}
	validate();
	return _servers;
}
