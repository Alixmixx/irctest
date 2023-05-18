#include "Server.hpp"

void Server::broadcast(std::vector<Client *> recipients, std::string message)
{
	this->broadcast(recipients, message, std::vector<Client *>());
}

void Server::broadcast(std::vector<Client *> recipients, std::string message, Client *except)
{
	std::vector<Client *> excepts;
	excepts.push_back(except);
	this->broadcast(recipients, message, excepts);
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