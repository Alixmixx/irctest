#include "Server.hpp"

void Server::handleQuit(Client* client, std::vector<std::string> arguments)
{
	std::string quitMessage = client->getPrefix() + " QUIT" + (arguments.size() > 0 ? " :" + arguments[0] : "");
	if (client->isRegistered())
		broadcast(_clients, quitMessage);
	removeClient(client);
}