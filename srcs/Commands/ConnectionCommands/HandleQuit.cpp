#include "Server.hpp"

void Server::handleQuit(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() == 0)
		replyMessage(client, "QUIT", " ");

	replyMessage(client, "QUIT", arguments[0]);
}