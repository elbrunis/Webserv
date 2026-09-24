#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"

class	Client
{
	public:
		int			fd;// creo q no se utiliza
		std::string readbuffer;
		std::string writebuffer;
		int			bitesent;
		bool		headersComplete;
};

#endif