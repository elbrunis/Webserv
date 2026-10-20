#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

#include "../Headers.hpp"
#include "ConfigStructs.hpp"

class ConfigParser
{
	public:

		ConfigParser(): _index(0){};
		~ConfigParser(){};

		std::vector<ServerConfig> 	parse(const std::string& filepath);

	private:

		std::vector<std::string>	_tokens;
		size_t						_index;
		std::vector<ServerConfig>	_servers;
		std::vector<LocationConfig>	_locations;

		//tokenize
		void						tokenize(const std::string& filepath);
		void						readLine(std::string& line);

		// parse
		ServerConfig				parseServer();
		bool						parseServerLoop(ServerConfig& s, std::string& key, std::vector<std::string>& args);
		LocationConfig				parseLocation();
		void						applyInheritance(ServerConfig& s);

		//token_utils
		const std::string&			nextToken();
		const std::string&			peekToken();
		bool						expect(std::string s);
		std::vector<std::string>	parseArgs();
		bool 						hasExactSize(const std::vector<std::string>& v, size_t n);

		// utils
		size_t						toNum(const std::string& s);
		void						validate() const;
};

#endif
