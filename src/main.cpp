#include "../inc/Headers.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Config/ConfigParser.hpp"
#include "../inc/Config/ConfigStructs.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc > 2)
			return 1;	// demasiados argumentos
		std::string path = (argc == 2) ? argv[1] : "config/default.conf";
		ConfigParser parse;
		std::vector<ServerConfig> _servers = parse.parse(path);
		Server server(8080); // de momento hardcodeado, luego vendrá del config
		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}