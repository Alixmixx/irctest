#include "Server.hpp"

void Server::broadcast(std::vector<Client *> recipients, std::string message)
{
	message += "\r\n";
	for (std::vector<Client *>::iterator it = recipients.begin(); it != recipients.end(); it++)
	{
		Client *client = *it;
		send(client->getSocket(), message.c_str(), message.length(), 0);
		if (DEBUG)
			std::cout << "\033[1;32mMessage to client: " << client->getSocket() << "\n"
					  << message << "\033[0m";
	}
}

void Server::broadcast(std::vector<Client *> recipients, std::string message, Client *except)
{
	message += "\r\n";
	for (std::vector<Client *>::iterator it = recipients.begin(); it != recipients.end(); it++)
	{
		Client *client = *it;
		if (client == except)
			continue;
		send(client->getSocket(), message.c_str(), message.length(), 0);
		if (DEBUG)
			std::cout << "\033[1;32mMessage to client: " << client->getSocket() << "\n"
					  << message << "\033[0m";
	}
}

void Server::broadcast(std::vector<Client *> recipients, std::string message, std::vector<Client *> except)
{
	message += "\r\n";
	for (std::vector<Client *>::iterator it = recipients.begin(); it != recipients.end(); it++)
	{
		Client *client = *it;
		if (std::find(except.begin(), except.end(), client) != except.end())
			continue;
		send(client->getSocket(), message.c_str(), message.length(), 0);
		if (DEBUG)
			std::cout << "\033[1;32mMessage to client: " << client->getSocket() << "\n"
					  << message << "\033[0m";
	}
}