#include "Server.hpp"

void Server::handlePing(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
		client->reply(ERR_NEEDMOREPARAMS, "PING");
	else
		client->reply(":" + _serverHostname + " PONG " + _serverHostname + " :" + arguments[0]);
}