#include "Server.hpp"

void Server::handleWhois(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		replyMessage(client, "ERR_NEEDMOREPARAMS", "WHOIS");
		return;
	}

	if (arguments.size() == 1)
	{
		Client *target = getClient(arguments[0]);
		if (!target)
		{
			replyMessage(client, "ERR_NOSUCHNICK", arguments[0]);
			return;
		}
		replyMessage(client, "RPL_WHOISUSER", target->getNickname(), target->getUsername(), target->getRealname());
		replyMessage(client, "RPL_ENDOFWHOIS", target->getNickname());
	}

	if (arguments.size() >= 2)
	{
		if (arguments[0] != client->getServer()->getServerName())
		{
			replyMessage(client, "ERR_NOSUCHSERVER", arguments[0]);
			return;
		}

		Client *target = getClient(arguments[1]);
		if (!target)
		{
			replyMessage(client, "ERR_NOSUCHNICK", arguments[1]);
			return;
		}
		// replyMessage(client, "RPL_WHOISSERVER"); // TODO
	}
}