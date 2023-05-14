#include "Server.hpp"

void Server::handlePing(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		replyMessage(client, "ERR_NEEDMOREPARAMS", "PING");
		return ;
	}
	replyMessage(client, "PONG", arguments[0]);
}