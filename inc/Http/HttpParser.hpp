#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

#include <string>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class RequestParser
{
	public:
		RequestParser() : _status(0), _state(REQUEST_LINE), _need(0){}; // Pendiente: da error al inicializarlo
		bool 	feed(const char* data, size_t n);
		int		statusCode();
	private:
		enum	State {REQUEST_LINE, HEADERS, BODY, ERROR, CHUNK_SIZE, CHUNK_DATA, CHUNK_TRAILER, CONTENT_LENGTH, DONE};
		bool	nextLine();
		void	parseRequestLine();
		bool	error(int n);
		void	parseHeader();
		void	startBody();
		// utils
		bool	processState();
		bool	processRequestLine();
		bool	processHeaders();
		bool	processContentLength();
		bool	processChunkSize();
		bool	processChunkData();
		bool	processChunkTrailer();

		std::string		_buff;
		std::string		_line;
		HTTPRequest		_request;
		int				_status;
		State			_state;
		int				_need;
};

#endif