#include "Server.hpp"

void Server::parseMessageFromClient(Client *client, std::string message)
{
	std::string command;
	std::string argument;
	std::vector<std::string> arguments;
	std::istringstream iss(message);

	iss >> command;
	std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(command); // find command in map

	if (it != _commandHandlers.end()) // si on trouve la commande dans la map des commandes
	{
		std::string argument;
		while (iss >> argument)
		{
			arguments.push_back(argument);
		}
		CommandHandler handler = it->second;
		(this->*handler)(client, arguments);
	}
	else
	{
		std::cout << "Command not found: " << command << std::endl;	// si c'est pas une commande c'est un message, donc on l'affiche sur le channel (si le client est dans un channel)
	}
}

void Server::readFromClient(Client *client)
{
	std::string message = fullRead(client->getSocket());

	while (message.find("\r\n") != std::string::npos)
	{
		std::string line = message.substr(0, message.find("\r\n"));
		message = message.substr(message.find("\n") + 1);
		parseMessageFromClient(client, line);
	}
}
