#include "../../inc/Http/HttpParser.hpp"
#include <cerrno>
#include <cstdlib>
#include <cctype>

bool RequestParser::feed(const char* data, size_t n)
{
    _buff.append(data, n);
    bool progress = true;

    while (progress && _state != ERROR && _state != DONE)
        progress = processState();

    if ((_state == REQUEST_LINE || _state == HEADERS) && _buff.size() > (KB * 8))
        return error(431);

    return _state == DONE || _state == ERROR;
}

bool RequestParser::processState()
{
    switch (_state)
    {
        case REQUEST_LINE:
            return processRequestLine();

        case HEADERS:
            return processHeaders();

        case CONTENT_LENGTH:
            return processContentLength();

        case CHUNK_SIZE:
            return processChunkSize();

        case CHUNK_DATA:
            return processChunkData();

        case CHUNK_TRAILER:
            return processChunkTrailer();

        default:
            return false;
    }
}

bool RequestParser::processRequestLine()
{
    if (!nextLine())
        return false;

    parseRequestLine();
    return true;
}

bool RequestParser::processHeaders()
{
    if (!nextLine())
        return false;

    if (_line.empty())
        startBody();
    else
        parseHeader();

    return true;
}

bool RequestParser::processContentLength()
{
    if (_buff.size() < static_cast<size_t>(_need))
        return false;

    _request.body.append(_buff, 0, _need);
    _buff.erase(0, _need);
    _state = DONE;

    return true;
}

bool RequestParser::processChunkSize()
{
    if (!nextLine())
        return false;
    errno = 0;
    bool allHex = !_line.empty();
    for (size_t i = 0; i < _line.size(); ++i)
        if (!std::isxdigit(static_cast<unsigned char>(_line[i])))
            {allHex = false; break;}
    if (!allHex)
        return error(400);

    _need = std::strtoul(_line.c_str(), NULL, 16);

    if (_request.body.size() + _need > MAX_BODY_SIZE || errno == ERANGE)
        return error(413);

    if (_need == 0)
        _state = CHUNK_TRAILER;
    else
        _state = CHUNK_DATA;

    return true;
}

bool RequestParser::processChunkData()
{
    if (_buff.size() < static_cast<size_t>(_need) + 2)
        return false;

    if (_buff.compare(_need, 2, "\r\n") != 0)
        return error(400);

    _request.body.append(_buff, 0, _need);
    _buff.erase(0, _need + 2);

    _state = CHUNK_SIZE;

    return true;
}

bool RequestParser::processChunkTrailer()
{
    if (!nextLine())
        return false;

    if (_line.empty())
        _state = DONE;

    return true;
}
