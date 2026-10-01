#include "../../inc/Utils/Utils.hpp"
# include "../../inc/Headers.hpp"

// estoy modificando la original igual haga falta cambiarlo
std::string to_lower(std::string s)   // se pasa por valor (copia)
{
	for (size_t i = 0; i < s.size(); i++)
		s[i] = std::tolower(static_cast<unsigned char>(s[i]));
	return s;
}

std::string	trim(const std::string& s)
{
	size_t first = s.find_first_not_of(" \t");
	if (first == std::string::npos)
		return "";
	size_t last = s.find_last_not_of(" \t");
	return s.substr(first, last - first + 1);
}

