#include "Server.hpp"

void Server::handlePart(Client* client, std::vector<std::string> argument)
{
	if (argument.size() < 1)
		return client->reply(ERR_NEEDMOREPARAMS, "PART");

	std::string reason = "";
	if (argument.size() > 1)
		reason = " :" + argument[1];

	std::vector<std::string> channels = split(argument[0], ',');
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel* channel = getChannel(*it);
		if (channel == NULL)
		{
			client->reply(ERR_NOSUCHCHANNEL, (*it));
			continue;
		}
		if (channel->isOnChannel(client) == false)
		{
			client->reply(ERR_NOTONCHANNEL, (*it));
			continue;
		}
		broadcast(channel->getChannelUsers(), client->getPrefix() + " PART " + channel->getName() + reason);
		channel->removeClientFromChannel(client);
	}
}
