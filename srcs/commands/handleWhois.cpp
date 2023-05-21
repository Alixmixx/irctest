#include "Server.hpp"

void Server::handleWhois(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
		return client->reply(ERR_NEEDMOREPARAMS, "WHOIS");

	if (arguments.size() == 1)
	{
		Client*	 target = getClient(arguments[0]);
		Channel* channel = getChannel(arguments[0]);

		if (target)
		{
			client->reply(RPL_WHOISUSER, target->getNickname(), target->getUsername(), target->getHostname(), target->getRealname());
			client->reply(RPL_WHOISSERVER, target->getNickname(), _serverName, _serverInfo);
			//client->reply(RPL_WHOISCHANNELS, target->getNickname(), target->getChannelList());
			//client->reply(RPL_WHOISIDLE, target->getNickname(), formatTime(target->getIdleTime()), formatTime(target->getSignOnTime()));
			return client->reply(RPL_ENDOFWHOIS, target->getNickname());
		}
		else if (channel)
		{
			//client->reply(RPL_WHOISCHANNELS, channel->getName(), channel->getChannelUsers());
			return client->reply(RPL_ENDOFWHOIS, channel->getName());
		}
		else
			return client->reply(ERR_NOSUCHNICK, arguments[0]);
	}

	if (arguments.size() > 1)
	{
		if (toLowerCase(arguments[0]) != toLowerCase(_serverName))
			return client->reply(ERR_NOSUCHSERVER, arguments[0]);

		Client* target = getClient(arguments[1]);
		if (!target)
		{
			client->reply(ERR_NOSUCHNICK, arguments[1]);
			return;
		}
		// client->reply(RPL_WHOISSERVER); // TODO
	}
}