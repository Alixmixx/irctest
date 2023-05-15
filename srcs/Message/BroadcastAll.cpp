#include "Server.hpp"

void Server::broadcastAll(std::string message)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *client = *it;
		send(client->getSocket(), message.c_str(), message.length(), 0);
	}
}