#include "Server.hpp"

void Server::handlePing(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		client->reply("ERR_NEEDMOREPARAMS", "PING");
		return ;
	}
	client->reply("PONG", arguments[0]);
}