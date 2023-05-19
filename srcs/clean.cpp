#include "ft_irc.hpp"

extern int exitStatus;

void handleSigint(int signum)
{
	(void)signum;
	exitStatus = EXIT_SUCCESS;
	std::cout << "\r" << BLUE << "Good bye. 💞" << RESET << std::endl;
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
