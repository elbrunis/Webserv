#include "../../inc/Config/ConfigParser.hpp"
#include "../../inc/Headers.hpp"

void	ConfigParser::readLine(std::string& line)
{
    std::string word;

	for (size_t i = 0; i < line.size(); i++)
	{
		char c = line[i];
		if (c == '{' || c == '}' || c == ';' || std::isspace(c))
		{

			if (!word.empty())
			{
				_tokens.push_back(word);
				word.erase();
			}
			if (!std::isspace(c))
				_tokens.push_back(std::string(1, c));
		}
		else
			word += c;
	}
	if (!word.empty())
		_tokens.push_back(word);
}

void	ConfigParser::tokenize(const std::string& filepath)
{
	std::ifstream file(filepath.c_str());
    if (!file.is_open())
        throw std::runtime_error("config: no se puede abrir " + filepath);

    std::string line;
    while (std::getline(file, line))
	{
		size_t hash = line.find('#');
		if (hash != std::string::npos)
			line.erase(hash);
		this->readLine(line);
	}
}