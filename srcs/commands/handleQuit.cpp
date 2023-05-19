#include "Server.hpp"

void Server::handleQuit(Client* client, std::vector<std::string> arguments)
{
	std::string clientMessage;
	std::string quitMessage = ":" + client->getNickname() + " QUIT";
	removeClient(client);
	if (arguments.size() >= 1)
		quitMessage += " :" + arguments[0];
	broadcast(_clients, quitMessage);
}