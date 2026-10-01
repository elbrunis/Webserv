#include "../../inc/Http/HttpParser.hpp"

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