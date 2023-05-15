#include "Server.hpp"

void Server::handleMode(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() == 0)
	{
		replyMessage(client, "ERR_NEEDMOREPARAMS", "MODE");
		return ;
	}

	if (client->getServer()->getClient(arguments[0]) == NULL)
	{
		replyMessage(client, "ERR_NOSUCHNICK", arguments[0]);
		return ;
	}

	if (client->getNickname() != arguments[0])
	{
		replyMessage(client, "ERR_USERSDONTMATCH");
		return ;
	}

	if (arguments.size() == 1)
	{
		//send modes TODO
	}
	else if (arguments.size() == 2)
	{
		//aplly arguments[1] mode  TODO
	}
	else
	{
		replyMessage(client, "ERR_NEEDMOREPARAMS", "MODE");
		return ;
	}
}