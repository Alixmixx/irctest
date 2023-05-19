#include "ft_irc.hpp"

extern Server *g_server;

void handleSigint(int signum)
{
	(void)signum;
	std::cout << "\r" << BLUE << "Good bye. 💞" << RESET << std::endl;
	delete g_server;
	exit(EXIT_SUCCESS);
}
