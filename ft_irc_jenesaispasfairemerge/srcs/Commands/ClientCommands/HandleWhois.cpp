#include "Server.hpp"

void Server::handleWhois(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		sendErrorMessage(client, "ERR_NEEDMOREPARAMS");
		return ;
	}

	if (arguments.size() == 1)
	{
		Client *target = getClient(arguments[0]);

		if (!target)
		{
			sendErrorMessage(client, "ERR_NOSUCHNICK");
			return ;
		}

		sendReplyMessage(client, "RPL_WHOISUSER", target->getNickname(), target->getUsername(), target->getRealname());
	}

	if (arguments.size() >= 2)
	{
		if (arguments[0] != client->getServer()->getServerName())
		{
			sendErrorMessage(client, "ERR_NOSUCHSERVER");
			return ;
		}

		Client *target = getClient(arguments[1]);

		if (!target)
		{
			sendErrorMessage(client, "ERR_NOSUCHNICK");
			return ;
		}

		sendReplyMessage(client, "RPL_WHOISSERVER");
	}
}