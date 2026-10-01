#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

#include "../Headers.hpp"
#include "../Client.hpp"
#include "../Utils/Utils.hpp"


struct HTTPRequest
{
	std::string method, uri, version;
	std::map<std::string, std::string> headers;
	std::string body;
};

class RequestParser
{
	public:
		RequestParser(); // por algun motivo cuando lo inicio da error
		bool feed(std::string str);
	private:
		enum	State {REQUEST_LINE, HEADERS, BODY, ERROR, CHUNK_SIZE, CHUNK_DATA, CHUNK_TRAILER, CONTENT_LENGTH, DONE};
		bool	nextLine();
		void	parseRequestLine();
		bool	error(int n);
		void	parseHeader();
		void	startBody();
		bool 	feed(const char* data, size_t n);

		Client& 	client;
		std::string	_buff;
		std::string	_line;
		HTTPRequest	_request;
		int			_status;
		State		_state;
		int			_need;
};

#endif