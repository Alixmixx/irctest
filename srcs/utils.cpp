#include "ft_irc.hpp"

extern bool run;

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

std::string strjoin(const std::vector<std::string>& vec, char delim)
{
	std::string result;

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (i != 0)
			result += delim;
		result += vec[i];
	}
	return result;
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
	int n = std::atoi(port.c_str());
	return n >= 1024 && n <= 65535; // Port numbers 0 to 1024 are reserved for privileged services and designated as well-known ports. This list of port numbers are specified in RFC 1700.
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

int stoi(std::string& s)
{
    int n;
    std::istringstream(s) >> n;
    return n;
}
