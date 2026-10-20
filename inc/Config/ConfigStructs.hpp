#ifndef CONFIG_STRUCTS_HPP
# define CONFIG_STRUCTS_HPP

#include "../Headers.hpp"

struct LocationConfig
{
	std::string							path;
	std::string							root;
	std::string							index;
	std::set<std::string>				methods;
	bool								autoindex;
	std::string							uploadStore;
	int									redirectCode;
	std::string							redirectUrl;
	std::map<std::string, std::string>	cgi;
	size_t								maxBody;
	bool								hasMaxBody;
	LocationConfig(): autoindex(false), redirectCode(0), maxBody(0), hasMaxBody(false){}
};

struct ServerConfig
{
	std::string					host;
	int							port;
	std::string					root;
	std::string 				index;
	std::vector<std::string>	serverNames;
	size_t						maxBody;
	std::map<int, std::string>	errorPages;
	std::vector<LocationConfig>	locations;
	ServerConfig() : host("0.0.0.0"), port(-1), index("index.html"), maxBody(1000000){}
};

#endif
