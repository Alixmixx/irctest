#include "ft_irc.hpp"

bool isPortNumberCorrect(std::string port)
{
	if (port.size() == 0
		|| port.size() > 5
		|| port[0] == '0'
		|| port.find_first_not_of("0123456789") != std::string::npos)
		return false;
	int n = atoi(port.c_str());
	return n >= 0 && n <= 65535;
}

bool isPasswordCorrect(std::string password)
{
	for (size_t i = 0; i < password.size(); ++i)
		if (password[i] < 32 || password[i] > 126)
			return false;
	return true;
}