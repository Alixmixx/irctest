#include "Server.hpp"

static std::vector<std::string> splitMessage(std::string message)
{
	std::vector<std::string> arguments;
	std::string				 current;
	std::stringstream		 iss(message);

	for (size_t i = 0; i < message.size(); ++i)
	{
		char c = message[i];
		if (c == ' ')
		{
			if (!current.empty())
			{
				arguments.push_back(current);
				current = "";
			}
		}
		else if (c == ':' && current.empty())
		{
			arguments.push_back(message.substr(i + 1));
			return arguments;
		}
		else
			current += c;
	}
	if (!current.empty())
		arguments.push_back(current);
	return arguments;
}

void Server::parseMessageFromClient(Client* client, std::string message)
{
	std::vector<std::string> arguments = splitMessage(message);
	if (arguments.size() == 0)
	{
		std::cout << BLUE << "Empty line received." << RESET << std::endl;
		return;
	}
	std::string command = toUpperCase(arguments[0]);
	arguments.erase(arguments.begin());
	std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(command);
	if (it == _commandHandlers.end())
		return client->reply(ERR_UNKNOWNCOMMAND, command);
	CommandHandler handler = it->second;
	(this->*handler)(client, arguments);
}

// TODO tres tard ~ reflechir si IRSSI/netcat peut faire exploser le serveur avec des char choulous
void Server::readFromClient(Client* client)
{
	std::string message = client->getMessage();
	char		buffer[BUFFER_SIZE] = {0};
	int			recvSize;

	do
	{
		syscall(recvSize = recv(client->getSocket(), buffer, BUFFER_SIZE - 1, 0), "recv");
		buffer[recvSize] = '\0';
		message += buffer;
	} while (recvSize == BUFFER_SIZE - 1);

	if (message.empty())
	{
		std::vector<std::string> args;
		args.push_back("Empty message from client");
		handleQuit(client, args);
		return;
	}

	std::cout << RED << "Message from client " << client->getSocket() << ":\n"
			  << message << RESET;

	size_t pos;
	while ((pos = message.find("\r\n")) != std::string::npos)
	{
		std::string line = message.substr(0, pos);
		message = message.substr(pos + 2);
		parseMessageFromClient(client, line);
	}
	client->setMessage(message);
}
