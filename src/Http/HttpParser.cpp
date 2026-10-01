#include "../../inc/Http/HttpParser.hpp"

bool	RequestParser::error(int code){_status = code, _state = ERROR; return true;}

bool	RequestParser::nextLine()
{
	size_t r_pos = _buff.find("\r\n");
	if (r_pos == std::string::npos)
		return false;
	if (!_line.empty())
		_line.erase(0, _line.size());
	this->_line = this->_buff.substr(0, r_pos);
	this->_buff.erase(0, r_pos + 2);
	return true;
}

// estoy asumiendo q todo llega en minusculas ya que hay q pasarlo
// el 501 no lo he echo por q no lo entiendo
void	RequestParser::parseRequestLine()
{
	std::istringstream stream(this->_line);
	struct HTTPRequest& request = this->_request;
	std::string extra;

	if (!(stream >> request.method >> request.uri >> request.version) 
		|| (stream >> extra) || request.uri.empty() || 
		request.uri[0] != '/' || request.version.rfind("HTTP/", 0) != 0)
		{error(400); return;}

	if (request.uri.length() > MAX_URI_SIZE)
		{error(414); return;}

	if (request.method != "GET" && request.method != "POST" 
			&& request.method != "DELETE")
		{error(405); return;}

	if (request.version != "HTTP/1.1")
		{error(505); return;}
	_state = HEADERS;
	
}


void	RequestParser::parseHeader()
{
	size_t separator = this->_line.find_first_of(":", 0);
	if ((separator == std::string::npos) || (separator == 0))
		{error(400); return;}

	std::string name = to_lower(_line.substr(0, separator));
	if (name.find_first_of(" \t") != std::string::npos)
		{error(400); return;}
	
	std::string value = trim(this->_line.substr(separator + 1));
	if (this->_request.headers.find(name) != this->_request.headers.end())
		this->_request.headers[name] += ", " + value;
	else
		this->_request.headers[name] = value;
}

void	RequestParser::startBody()
{
	size_t host = this->_request.headers.count("host");
	size_t chunk = this->_request.headers.count("transfer-encoding");
	size_t length = this->_request.headers.count("content-length");

	if ((chunk && length) || !host)
		{error(400); return;}
	
	if (chunk)
	{
		std::string& value = this->_request.headers["transfer-encoding"];
		if (value.empty() || "chunked" != to_lower(value))
			{error(501); return;}
		_state = CHUNK_SIZE;
	}
	else if (length)
	{
		std::string& value = this->_request.headers["content-length"];
		if (value.empty() || !std::all_of(value.begin(), value.end(), ::isdigit))
			{error(400); return;}
		
		errno = 0;
		_need = std::strtoul(value.c_str(), NULL, 10);
		if (_need > MAX_BODY_SIZE || errno == ERANGE)
			{error(413); return;}
		if (_need == 0)
			_state = DONE;
		else
			_state = CONTENT_LENGTH;
	}
	else
		_state = DONE;
}

// NECESITAS ARREGLAR EL CONSTRUCTOR PRIMERO PARA Q FUNCIONE LA FUNCION
bool 	RequestParser::feed(const char* data, size_t n)
{
	this->_buff.append(data, n);
	bool progress = true;

	while (progress == true && _state != ERROR && _state != DONE)
	{
		progress  = false;
		switch (_state)
		{
			case REQUEST_LINE:
				if (nextLine())
					{parseRequestLine(); progress  = true;}
				break;
			case HEADERS:
				if (nextLine())
				{
					if (_line.empty())
						{startBody(); progress  = true;}
					else
						{parseHeader(); progress  = true;}
				}
				break;
			case CONTENT_LENGTH:
				if (_buff.size() >= _need)
				{
					_request.body.append(_buff, 0, _need);
					_buff.erase(0, _need);
					_state = DONE; progress  = true;
				}
				break;
			case CHUNK_SIZE:
				if(nextLine())
				{
					errno = 0;
					if (_line.empty() || !std::all_of(_line.begin(), _line.end(),  [](unsigned char c) { return std::isxdigit(c); }))
						return error(400);
					_need = std::strtoul(_line.c_str(), NULL, 16);
					if (_request.body.size() + _need > MAX_BODY_SIZE || errno == ERANGE)
						return error(413);
					if (_need == 0)
						{_state = CHUNK_TRAILER; progress  = true;}
					else
						{_state = CHUNK_DATA; progress  = true;}
				}
				break;
			case CHUNK_DATA:
				if(_buff.size() >= _need + 2)
				{
					if (_buff.compare(_need, 2, "\r\n") != 0) 
						return error(400);
					_request.body.append(_buff, 0, _need);
					_buff.erase(0, _need + 2);
					_state = CHUNK_SIZE; progress  = true;
				}
				break;
			case CHUNK_TRAILER:
				if (nextLine())
				{
					if (_line.empty())
						_state = DONE;
					progress = true;
				}
				break;
			default:break;		
		}
	}
	if ((_state == REQUEST_LINE || _state == HEADERS) && _buff.size() > (KB * 8))
		return error(431);
	return _state == DONE || _state == ERROR;
}
