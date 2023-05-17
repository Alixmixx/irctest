#include "ft_irc.hpp"

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

std::string getCurrentDateTime()
{
    std::time_t currentTime = std::time(NULL);
    std::tm* localTime = std::localtime(&currentTime);

    // Array of day names
    const char* dayNames[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    // Array of month names
    const char* monthNames[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    std::stringstream dateTimeStream;
    dateTimeStream << dayNames[localTime->tm_wday] << " "
                   << monthNames[localTime->tm_mon] << " "
                   << std::setfill('0') << std::setw(2) << localTime->tm_mday << " "
                   << std::setw(2) << localTime->tm_hour << ":"
                   << std::setw(2) << localTime->tm_min << ":"
                   << std::setw(2) << localTime->tm_sec << " "
                   << (localTime->tm_year + 1900);

    return dateTimeStream.str();
}

std::vector<std::string> split(const std::string &str, char delim)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delim))
	{
		if (token != "")
			result.push_back(token);
	}
	return result;
}
