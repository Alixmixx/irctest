#include "Server.hpp"

void Server::handleWhois(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS, "WHOIS");
		return;
	}

	if (arguments.size() == 1)
	{
		Client* target = getClient(arguments[0]);
		if (!target)
		{
			client->reply(ERR_NOSUCHNICK, arguments[0]);
			return;
		}
		client->reply(RPL_WHOISUSER, target->getNickname(), target->getUsername(), target->getHostname(), target->getRealname());
		client->reply(RPL_ENDOFWHOIS, target->getNickname());
	}

	if (arguments.size() >= 2)
	{
		if (arguments[0] != client->getServer()->getServerName())
		{
			client->reply(ERR_NOSUCHSERVER, arguments[0]);
			return;
		}

		Client* target = getClient(arguments[1]);
		if (!target)
		{
			client->reply(ERR_NOSUCHNICK, arguments[1]);
			return;
		}
		// client->reply(RPL_WHOISSERVER); // TODO
	}
}