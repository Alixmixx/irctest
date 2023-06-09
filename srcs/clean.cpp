#include "ft_irc.hpp"

extern bool run;

int argumentError(std::string message)
{
	std::cerr << message << std::endl;
	return ARGUMENT_ERROR;
}

void signalHandler(int signum)
{
	(void)signum;
	run = false;
}

void syscall(int returnValue, const char* funcName)
{
	if (returnValue < 0)
		throw SystemError(funcName);
}
