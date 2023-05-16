#include "Server.hpp"

std::string concatenateArguments(std::vector<std::string> arguments, unsigned int start)
{
	std::string message = "";

	if (arguments.size() <= start)
		return (message);

	std::vector<std::string>::iterator it = arguments.begin() + start;
	for (; it != arguments.end(); ++it)
	{
		message += *it;
		if (it + 1 != arguments.end())
			message += " ";
	}
	return (message);
}