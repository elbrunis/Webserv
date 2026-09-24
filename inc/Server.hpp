#ifndef SERVER_HPP
# define SERVER_HPP

# include "Headers.hpp"
# include "Client.hpp"

class	Server
{
	public:
		Server (int port);
		void	run(); //En un futuro aqui ira la config
	private:
		int							lisent_fd;
		std::vector<struct pollfd>	fds;
		std::map<int, Client>		clients;
		void	accept_new_client();
		int		read_client(int i);
		void 	write_client(int i);
		void	setupSocket(int port);

};

#endif