#include "Server.hpp"

void Server::handlePing(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		std::string asd;
		client->reply(ERR_NEEDMOREPARAMS, "PING");
		return ;
	}
	std::string pong = "PONG " + arguments[0] + "\r\n";
	send(client->getSocket(), pong.c_str(), pong.length(), 0);
}