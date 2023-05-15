#include "Server.hpp"
#include "Client.hpp"

static bool isPortNumberCorrect(std::string port)
{
	if (port.find_first_not_of("0123456789") != std::string::npos)
		return false;
	if (atoi(port.c_str()) < 0 || atoi(port.c_str()) > 65535)
		return false;
	return true;
}

int main(int argc, char **argv)
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
	}  // TODO maybe check if password is valid (por example pas de char bizarres dedans, dev random, env -i, etc etc etc)

	Server server(atoi(argv[1]), argv[2]);
	server.start();
	return EXIT_SUCCESS;
}