#include "Server.hpp"

void Server::handleQuit(Client *client, std::vector<std::string> arguments)
{
	std::string clientNickname = client->getNickname();
	std::string clientMessage;
	removeClient(client);

	if (arguments.size() == 0)
		broadcastAll(":" + clientNickname + " QUIT\r\n");

	for (std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); it++)
	{
		clientMessage += *it;
		if (it + 1 != arguments.end())
			clientMessage += " ";
	}

	broadcastAll(":" + clientNickname + " QUIT :" + clientMessage + "\r\n");
}