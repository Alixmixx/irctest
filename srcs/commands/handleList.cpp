#include "Server.hpp"

static bool shouldReplyChannel(Channel *channel, Client *client, std::vector<std::string> list)
{
	if (channel->isSecret() && !channel->isOnChannel(client))
		return false;
	if (list.empty())
		return true;
	return std::find(list.begin(), list.end(), toLowerCase(channel->getName())) != list.end();
}

void Server::handleList(Client *client, std::vector<std::string> arguments)
{
	client->reply(RPL_LISTSTART);

	std::vector<std::string> list = arguments.empty() ? std::vector<std::string>() : split(toLowerCase(arguments[0]), ',');
	for (std::vector<Channel *>::iterator channel = _channels.begin(); channel != _channels.end(); channel++)
		if (shouldReplyChannel(*channel, client, list))
			client->reply(RPL_LIST, (*channel)->getName(), toString((*channel)->getChannelUsers().size()), (*channel)->getTopic());

	client->reply(RPL_LISTEND);
}