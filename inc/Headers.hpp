#ifndef HEADERS_HPP
# define HEADERS_HPP

# include <vector>
# include <map>
# include <string>
# include <poll.h>
# include <unistd.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdexcept>
# include <iostream>
# include <cstring>
# include <sstream>
# include <algorithm>
# include <cctype>
# include "Http/HttpRequest.hpp"
# include "Http/HttpParser.hpp"
# include "Http/HttpParserState.hpp"
# include "Http/HttpResponse.hpp"


// tamaño total del buffer
# define BUFF_LEN 4096

//tamaño maximo de las uri
const size_t MAX_URI_SIZE = 4096;

// body max lenght
# define MAX_BODY_SIZE 10485760
# define KB 1024

#endif