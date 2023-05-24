#include "Server.hpp"

void Server::handleQuit(Client* client, std::vector<std::string> arguments)
{
	std::string message = arguments.empty() ? "" : "Quit: " + arguments[0];
	client->reply("ERROR :Closing Link: " + client->getIp() + " (" + message + ")");
	std::string quitMessage = client->getPrefix() + " QUIT :" + message;
	if (client->isRegistered())
	{
		FormerClient* formerClient =
			new FormerClient(client->getNickname(), client->getUsername(), client->getRealname(),
							 client->getHostname(), std::time(NULL));
		_formerClients.push_back(formerClient);
		broadcast(_clients, quitMessage, client); // TODO broadcast only to people on same channels
	}
	removeClient(client);
}