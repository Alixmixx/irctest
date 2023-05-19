#include "ft_irc.hpp"

int main(int argc, char** argv)
{
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

	Server server(atoi(argv[1]), argv[2]);
	server.start();
	return EXIT_SUCCESS;
}