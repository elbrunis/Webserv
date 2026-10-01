#include "../Headers.hpp"

class HTTPresponse
{
	public:
		HTTPresponse(int code) : _code(code), _host("/"){};
		void		setHeader(const std::string& n, const std::string& v);
		void		addToBody(std::string body){_body += body;}
		void		setHost(std::string host){_host = host;}
		std::string	toStr();

	private:
		int									_code;
		std::map<std::string, std::string>	_headers;
		std::string 						_body;
		std::string							_host;
		static const char* reason(int c) 
		{
			switch (c) 
			{
				case 200: return "OK";						case 201: return "Created";
				case 204: return "No Content";				case 301: return "Moved Permanently";
				case 400: return "Bad Request";				case 403: return "Forbidden";
				case 404: return "Not Found";				case 405: return "Method Not Allowed";
				case 413: return "Payload Too Large";		case 431: return "Request Header Fields Too Large";
				case 500: return "Internal Server Error";	case 501: return "Not Implemented";
				case 505: return "HTTP Version Not Supported";
				default: return "Unknown";
			}	
		}
};