#include "ft_irc.hpp"

bool run = true;

static int argumentError(std::string message)
{
	std::cerr << message << std::endl;
	return ARGUMENT_ERROR;
}

int main(int argc, char** argv)
{
	if (argc != 3)
		return argumentError("Usage: " + std::string(argv[0]) + " <port> <password>");
	else if (!isPortNumberCorrect(argv[1]))
		return argumentError("Invalid port number: " + std::string(argv[1]));
	else if (!isStringPrintable(argv[2]))
		return argumentError("Invalid password: " + std::string(argv[2]));
	Server server(atoi(argv[1]), argv[2]);
	std::signal(SIGINT, handleSigint);
	std::cout << BLUE << "Press Ctrl+C to exit." << RESET << std::endl;
	server.start();
	return EXIT_SUCCESS;
}