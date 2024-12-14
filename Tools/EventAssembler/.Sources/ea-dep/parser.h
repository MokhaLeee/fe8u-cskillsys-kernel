#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#include <string>
#include <vector>

struct Includes
{
	std::vector<std::string> includeFiles;
	std::vector<std::string> incbinFiles;
};

Includes parse_includes(const std::string& fileContents);

#endif // PARSER_INCLUDED
