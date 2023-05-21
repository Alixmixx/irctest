#include "ft_irc.hpp"

extern bool run;

void panic(std::string message)
{
	std::cerr << message << std::endl;
	std::exit(EXIT_FAILURE);
}

void syscall(int returnValue, const char* funcName)
{
	if (returnValue < 0)
		throw SystemError(funcName);
}

void signalHandler(int signum)
{
	(void)signum;
	run = false;
}
