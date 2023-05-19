#include "ft_irc.hpp"

extern Server *g_server;

void handleSigint(int signum)
{
	(void)signum;
	std::cout << "\r" << BLUE << "Good bye. 💞" << RESET << std::endl;
	delete g_server;
	exit(EXIT_SUCCESS);
}

void panic(std::string message)
{
	std::cerr << message << std::endl;
	delete g_server;
    exit(EXIT_FAILURE);
}

void syscall(int returnValue, const char *funcName)
{
    if (returnValue < 0)
	{
        std::perror(funcName);
        delete g_server;
        exit(EXIT_FAILURE);
    }
}