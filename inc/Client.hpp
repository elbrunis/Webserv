#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Headers.hpp"

class	Client
{
	public:
		int			fd;
		std::string readbuffer;
		std::string writebuffer;
		int			bitesent;
		bool		headersComplete;
};

#endif