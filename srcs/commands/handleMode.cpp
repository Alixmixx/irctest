#include "Server.hpp"

void Server::handleMode(Client* client, std::vector<std::string> arguments) // TODO by Alix
{
	if (arguments.size() == 0)
	{
		client->reply(ERR_NEEDMOREPARAMS, "MODE");
		return;
	}

	if (client->getServer()->getClient(arguments[0]) == NULL)
	{
		client->reply(ERR_NOSUCHNICK, arguments[0]);
		return;
	}

	if (client->getNickname() != arguments[0])
	{
		client->reply(ERR_USERSDONTMATCH);
		return;
	}

	if (arguments.size() == 1)
	{
		// send modes TODO
	}
	else if (arguments.size() == 2)
	{
		// aplly arguments[1] mode  TODO
	}
	else
	{
		client->reply(ERR_NEEDMOREPARAMS, "MODE");
		return;
	}
}