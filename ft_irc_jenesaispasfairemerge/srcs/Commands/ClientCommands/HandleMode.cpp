#include "Server.hpp"

void Server::handleMode(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() == 0)
	{
		sendErrorMessage(client, "ERR_NEEDMOREPARAMS");
		return ;
	}

	if (client->getServer()->getClient(arguments[0]) == NULL)
	{
		sendErrorMessage(client, "ERR_NOSUCHNICK");
		return ;
	}

	if (client->getNickname() != arguments[0])
	{
		sendErrorMessage(client, "ERR_USERSDONTMATCH");
		return ;
	}

	if (arguments.size() == 1)
	{
		//send modes
	}
	else if (arguments.size() == 2)
	{
		//aplly arguments[1] mode
	}
	else
	{
		sendErrorMessage(client, "ERR_NEEDMOREPARAMS");
		return ;
	}
}