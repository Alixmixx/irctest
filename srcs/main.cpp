#include "ft_irc.hpp"

bool run = true;

int main(int argc, char** argv)
{
	if (argc != 3)
		return argumentError("Usage: " + std::string(argv[0]) + " <port> <password>");
	if (!isPortNumberCorrect(argv[1]))
		return argumentError("Invalid port number: " + std::string(argv[1]));
	if (argv[2][0] == '\0' )
		return argumentError("Empty password");
	if (!isStringPrintable(argv[2]))
		return argumentError("Invalid password: " + std::string(argv[2]));

	Server server(std::atoi(argv[1]), argv[2]);
	try {
		server.init();
		std::cout << BLUE << "Listening on port " << server.getPort() << ". 👂" << RESET << std::endl;
		signal(SIGINT, signalHandler);
		std::cout << BLUE << "Press Ctrl+C to exit." << RESET << std::endl;
		server.loop();
		std::cout << BLUE << "\rGood bye. 💞" << RESET << std::endl;
		return EXIT_SUCCESS;
	} catch (const SystemError &e) {
        std::perror(e.funcName);
		return EXIT_FAILURE;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
		return OUTSTANDING_ERROR;
    }
}