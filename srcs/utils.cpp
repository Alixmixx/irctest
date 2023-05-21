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

std::vector<std::string> split(const std::string& str, char delim)
{
	std::vector<std::string> result;
	std::stringstream		 ss(str);
	std::string				 token;

	while (std::getline(ss, token, delim))
	{
		if (token != "")
			result.push_back(token);
	}
	return result;
}

std::string toString(long number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

std::string toString(Client* client)
{
	std::stringstream ss;
	ss << *client;
	return ss.str();
}

std::string formatTime(time_t time)
{
	std::tm* localTime = std::localtime(&time);

	const char* dayNames[] = {
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

	const char* monthNames[] = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};

	std::stringstream dateTimeStream;
	dateTimeStream << dayNames[localTime->tm_wday] << " "
				   << std::setfill('0') << std::setw(2) << localTime->tm_mday << " "
				   << monthNames[localTime->tm_mon] << " "
				   << (localTime->tm_year + 1900) << " at "
				   << std::setw(2) << localTime->tm_hour << ":"
				   << std::setw(2) << localTime->tm_min << ":"
				   << std::setw(2) << localTime->tm_sec;

	return dateTimeStream.str();
}

bool isPortNumberCorrect(std::string port)
{
	if (port.size() == 0 || port.size() > 5 || port[0] == '0' || port.find_first_not_of("0123456789") != std::string::npos)
		return false;
	int n = atoi(port.c_str());
	return n >= 0 && n <= 65535;
}

bool isStringPrintable(std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
		if (!std::isprint(str[i]))
			return false;
	return true;
}

std::string toLowerCase(std::string str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
		*it = std::tolower(*it);
	return str;
}

std::string toUpperCase(std::string str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
		*it = std::toupper(*it);
	return str;
}