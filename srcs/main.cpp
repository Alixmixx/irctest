#include "ft_irc.hpp"

Server *g_server = NULL;

int main(int argc, char** argv)
{
	std::signal(SIGINT, handleSigint);
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return EXIT_FAILURE;
	}
	else if (!isPortNumberCorrect(argv[1]))
	{
		std::cerr << "Invalid port number: " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}
	else if (!isStringPrintable(argv[2]))
	{
		std::cerr << "Invalid password: " << argv[2] << std::endl;
		return EXIT_FAILURE;
	}
	g_server = new Server(atoi(argv[1]), argv[2]);
	g_server->start();
	return OUTSTANDING_ERROR;
}