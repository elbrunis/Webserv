#include "../inc/Headers.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

int main()
{
	try
	{
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