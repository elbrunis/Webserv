#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "Http/HttpRequest.hpp"
#include "Http/HttpResponse.hpp"
#include "Http/HttpParser.hpp" 

class	Client
{
	private:
	public:
		int				fd;// creo q no se utiliza
		std::string 	readbuffer;
		std::string 	writebuffer;
		int				bitesent;
		bool			headersComplete;
		RequestParser	parser;
		HTTPResponse	response;
};

#endif