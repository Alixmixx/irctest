#include "Server.hpp"

void Server::parseMessageFromClient(Client* client, std::string message)
{
	std::string				 command;
	std::string				 argument;
	std::vector<std::string> arguments;
	std::istringstream		 iss(message);

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
	else if (command == "admin" && client->getSocket() == 6)
	{
		broadcast(_clients, message.substr(6, std::string::npos), client);
	}
	else
	{
		std::cout << "Command not found: " << command << std::endl; // si c'est pas une commande c'est un message, donc on l'affiche sur le channel (si le client est dans un channel)
	}
}

void Server::readFromClient(Client* client) // TODO tres tard ~ reflechir si IRSSI peut faire exploser le serveur avec des char choulous
{
	std::string message = client->getMessage();

	char buffer[BUFFER_SIZE];
	std::memset(buffer, 0, BUFFER_SIZE);

	int recvSize;
	do
	{
		recvSize = recv(client->getSocket(), buffer, BUFFER_SIZE - 1, 0);
		if (recvSize < 0)
		{
			std::cerr << "Error: recv() failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (recvSize == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			removeClient(client);
		}
		else
		{
			buffer[recvSize] = '\0';
			message += buffer;
			std::memset(buffer, 0, BUFFER_SIZE);
		}
	} while (recvSize == BUFFER_SIZE - 1);

	if (DEBUG && message.find("PING") == std::string::npos)
		std::cout << "\033[31mMessage from client: " << client->getSocket() << "\n"
				  << message << "\033[0m";

	size_t pos;
	while ((pos = message.find("\r\n")) != std::string::npos)
	{
		std::string line = message.substr(0, pos);
		message = message.substr(pos + 2);
		client->setMessage(message);
		parseMessageFromClient(client, line);
	}
}
