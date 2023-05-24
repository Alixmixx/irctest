#include "Server.hpp"

static void showChannelUsers(Channel* channel, Client* client, bool showInvisible)
{
	std::string symbol = "=";
	std::string names = "";

	if (channel->isSecret() && channel->isOnChannel(client))
		symbol = "@";
	else if (channel->isSecret())
		return;
	else if (channel->isInviteOnly())
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
		case VOICE:
			names += "+";	// voice sur irssi
			break;
		case MODERATOR:
			names += "%";	// halfop sur irssi
			break;
		case OPERATOR:
			names += "@";	// op sur irssi
			break;
		case FOUNDER:
			names += "@";	// owner sur irssi mais donc toujours @ à la place de ~
			break;
		default:
			names += "";	// user sur irssi
			break;
		}
		names += it->first->getNickname();
		it++;
	}

	if (names != "")
		client->reply(RPL_NAMREPLY, symbol, channel->getName(), names);
}

void Server::handleNames(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
		return client->reply(ERR_NEEDMOREPARAMS, "NAMES");

	std::vector<std::string> channels = split(arguments[0], ',');

	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel* channel = getChannel(*it);
		if (channel == NULL || channel->getChannelUsers().size() == 0)
		{
			client->reply(RPL_ENDOFNAMES, (*it));
			continue;
		}

		if (channel->isOnChannel(client) == false && (channel->isSecret() == true))
		{
			client->reply(RPL_ENDOFNAMES, (*it));
			continue;
		}

		showChannelUsers(channel, client, channel->isOnChannel(client));
		client->reply(RPL_ENDOFNAMES, channel->getName());
	}
}