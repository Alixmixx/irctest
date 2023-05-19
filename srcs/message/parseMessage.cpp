#include "Server.hpp"

static std::vector<std::string> splitMessage(std::string message)
{
    std::vector<std::string> arguments;
    std::string current;
    std::stringstream iss(message);

    for (size_t i = 0; i < message.size(); ++i) {
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
	std::string command = arguments[0];
	arguments.erase(arguments.begin());
	std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(command);
	if (it != _commandHandlers.end())
	{
		CommandHandler handler = it->second;
		(this->*handler)(client, arguments);
	}
	else
	{
		// TODO si c'est pas une commande c'est un message, donc on l'affiche sur le channel (si le client est dans un channel)
		std::cout << BLUE <<  "Command not found: " << command << RESET << std::endl;
	}
}

// TODO tres tard ~ reflechir si IRSSI peut faire exploser le serveur avec des char choulous
void Server::readFromClient(Client* client)
{
	std::string message = client->getMessage();
	char buffer[BUFFER_SIZE] = {0};
	int recvSize;

	do
	{
		syscall(recvSize = recv(client->getSocket(), buffer, BUFFER_SIZE - 1, 0), "recv");
		if (recvSize == 0)
		{
			std::cout << BLUE << "Client disconnected." << RESET << std::endl;
			removeClient(client);
		}
		else
		{
			buffer[recvSize] = '\0';
			message += buffer;
			std::memset(buffer, 0, BUFFER_SIZE);
		}
	} while (recvSize == BUFFER_SIZE - 1);

	std::cout << RED << "Message from client " << client->getSocket() << ":\n" << message << RESET;

	size_t pos;
	while ((pos = message.find("\r\n")) != std::string::npos)
	{
		std::string line = message.substr(0, pos);
		message = message.substr(pos + 2);
		client->setMessage(message);
		parseMessageFromClient(client, line);
	}
}
