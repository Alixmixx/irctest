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
			// peut-etre ça sert a rien, parceque les commandes finissent par \r\n donc deja geré par le readFromClient. Mais faut surement gerer des commandes qui n'ont pas d'arguments
			if (_commandHandlers.find(argument) != _commandHandlers.end())
				break;

			arguments.push_back(argument);
		}

		if (DEBUG)
		{
			std::cout << "Command found: " << command << std::endl;
			std::cout << "Arguments: " << arguments.size() << std::endl;
			for (std::vector<std::string>::const_iterator argIt = arguments.begin(); argIt != arguments.end(); ++argIt)
				std::cout << *argIt << " ";
			std::cout << std::endl;
		}

		CommandHandler handler = it->second;
		(this->*handler)(client, arguments);
	}
	else
	{
		std::cout << "Command not found: " << command << std::endl;
		//std::cout << iss << std::endl; // si c'est pas une commande c'est un message, donc on l'affiche sur le channel (si le client est dans un channel)
	}
}

void Server::readFromClient(Client *client)
{
	std::string message = fullRead(client->getSocket());
	if (DEBUG)
		std::cout << "\nMessage from client " << client->getNickname() << ": " << message << "\n" << std::endl;

	while (message.find("\r\n") != std::string::npos)
	{
		std::string line = message.substr(0, message.find("\r\n"));
		message = message.substr(message.find("\n") + 1);
		parseMessageFromClient(client, line);
	}
}
