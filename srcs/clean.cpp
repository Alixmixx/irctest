#include "ft_irc.hpp"

extern bool run;

void handleSigint(int signum)
{
	(void)signum;
	if (run)
	{
		run = false;
		std::cout << "\r" << BLUE << "Good bye. 💞" << RESET << std::endl;
	}
}

void panic(std::string message)
{
	std::cerr << message << std::endl;
	std::exit(EXIT_FAILURE);
}

void syscall(int returnValue, const char* funcName)
{
	if (returnValue < 0)
	{
		std::perror(funcName);
		std::exit(EXIT_FAILURE);
	}
}
