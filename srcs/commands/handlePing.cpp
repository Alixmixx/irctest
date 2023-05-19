#include "Server.hpp"

void Server::handlePing(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		std::string asd;
		client->reply(ERR_NEEDMOREPARAMS, "PING");
		return;
	}
	client->reply("PONG " + arguments[0]);
}