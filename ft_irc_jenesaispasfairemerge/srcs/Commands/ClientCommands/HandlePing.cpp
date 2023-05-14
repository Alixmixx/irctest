#include "Server.hpp"

void Server::handlePing(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		sendErrorMessage(client, "ERR_NEEDMOREPARAMS");
		return ;
	}

	std::string fullMessage = "PONG " + arguments[0] + "\r\n";
	send(client->getSocket(), fullMessage, fullMessage.length(), 0);