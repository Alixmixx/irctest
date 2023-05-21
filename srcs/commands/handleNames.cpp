#include "Server.hpp"

static void showChannelUsers(Channel* channel, Client* client, bool showInvisible)
{
	std::string symbol = "=";
	std::string names = ":"; // peut etre pas

	if (channel->isSecret() == true)
		symbol = "@";
	else if (channel->isInviteOnly() == true)
		symbol = "*";

	std::map<Client*, int>::iterator it = channel->getChannelUsersModes().begin();
	while (it != channel->getChannelUsersModes().end())
	{
		if (it != channel->getChannelUsersModes().begin())
			names += " ";
		if (it->first->isInvisible() && !showInvisible)
		{
			it++;
			continue;
		}
		if (it->second == BANNED || it->second == NOTINCHANNEL || it->second == INVITED)
		{
			it++;
			continue;
		}
		switch (it->second)
		{
		case MODERATOR:
			names += "%";
			break;
		case OPERATOR:
			names += "@";
			break;
		case PROTECTED:
			names += "&";
			break;
		case FOUNDER:
			names += "~";
			break;
		default:
			names += "+";
			break;
		}
		names += it->first->getNickname();
		it++;
	}

	//<client>                   <symbol><channel>           :[prefix]<nick>{ [prefix]<nick>}"
	// client->reply(RPL_NAMREPLY, symbol, channel->getName(), names);
	// "<client> <channel> :End of /NAMES list"
	client->reply(RPL_ENDOFNAMES, channel->getName());
}

void Server::handleNames(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
		return client->reply(ERR_NEEDMOREPARAMS, "NAMES");
	if (arguments[0].find(',') != std::string::npos)
		return client->reply(RPL_ENDOFNAMES, "TARGMAX 1");
	Channel* channel = getChannel(arguments[0]);
	if (channel == NULL || channel->getChannelUsers().size() == 0)
		return client->reply(RPL_ENDOFNAMES, arguments[0]);
	if (channel->isOnChannel(client) == false && channel->isSecret())
		return client->reply(RPL_ENDOFNAMES, arguments[0]);
	showChannelUsers(channel, client, channel->isOnChannel(client));
}