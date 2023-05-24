#include "Server.hpp"

void Server::handleQuit(Client* client, std::vector<std::string> arguments)
{
	std::string message = arguments.empty() ? "" : "Quit: " + arguments[0];
	client->reply("ERROR :Closing Link: " + client->getIp() + " (" + message + ")");
	std::string quitMessage = client->getPrefix() + " QUIT :" + message;
	if (client->isRegistered())
		broadcast(_clients, quitMessage, client);
	removeClient(client);
}