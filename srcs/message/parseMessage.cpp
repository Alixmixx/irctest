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

static bool canExecute(Client* client, std::string command)
{
	if (command == "PASS")
		return true;
	else if (command == "NICK" || command == "USER")
		return client->isPasswordCorrect();
	else
		return client->isRegistered();
}

void Server::parseMessageFromClient(Client* client, std::string message)
{
	std::vector<std::string> arguments = splitMessage(message);
	if (arguments.size() == 0)
	{
		std::cout << BLUE << "Empty line received." << RESET << std::endl;
		return;
	}
	std::string command = arguments[0];
	if (command == "CAP" || command == "PONG")
		return;
	if (!canExecute(client, command))
		return client->reply(ERR_NOTREGISTERED);
	if (command != "PING")
		client->setLastAction();
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
	char buffer[BUFFER_SIZE] = {0};
	int	 recvSize;

	do
	{
		syscall(recvSize = recv(client->getSocket(), buffer, BUFFER_SIZE - 1, 0), "recv");
		buffer[recvSize] = '\0';
		client->_message += buffer;
	} while (recvSize == BUFFER_SIZE - 1);

	if (client->_message.empty())
	{
		std::vector<std::string> args;
		args.push_back("Connection lost with client");
		return handleQuit(client, args);
	}

	if (client->_message.find("PING") == std::string::npos)
		std::cout << RED << "Message from client " << client->getSocket() << ":\n"
				  << client->_message << RESET;

	size_t pos;
	while ((pos = client->_message.find("\r\n")) != std::string::npos)
	{
		std::string line = client->_message.substr(0, pos);
		client->_message = client->_message.substr(pos + 2);
		parseMessageFromClient(client, line);
	}
}
