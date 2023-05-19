#include "Server.hpp"

void Server::handleQuit(Client* client, std::vector<std::string> arguments)
{
	std::string clientNickname = client->getNickname();
	std::string clientMessage;
	removeClient(client);

	if (arguments.size() == 0)
	{
		broadcast(_clients, ":" + clientNickname + " QUIT");
		return;
	}

	for (std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); it++)
	{
		clientMessage += *it;
		if (it + 1 != arguments.end())
			clientMessage += " ";
	}

	broadcast(_clients, ":" + clientNickname + " QUIT " + clientMessage);
}