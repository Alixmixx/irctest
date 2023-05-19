#include "Server.hpp"

void Server::broadcast(std::vector<Client*> recipients, std::string message)
{
	this->broadcast(recipients, message, std::vector<Client*>());
}

void Server::broadcast(std::vector<Client*> recipients, std::string message, Client* except)
{
	std::vector<Client*> excepts;
	excepts.push_back(except);
	this->broadcast(recipients, message, excepts);
}

void Server::broadcast(std::vector<Client*> recipients, std::string message, std::vector<Client*> except)
{
	for (std::vector<Client*>::iterator it = recipients.begin(); it != recipients.end(); it++)
	{
		Client* client = *it;
		if (std::find(except.begin(), except.end(), client) != except.end())
			continue;
		client->reply(message);
	}
}