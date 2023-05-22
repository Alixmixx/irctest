#include "Server.hpp"

void Server::handleTopic(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
		return client->reply(ERR_NEEDMOREPARAMS, "TOPIC");

	Channel* channel = getChannel(arguments[0]);
	if (channel == NULL)
		return client->reply(ERR_NOSUCHCHANNEL, arguments[0]);

	if (!channel->isOnChannel(client))
		return client->reply(ERR_NOTONCHANNEL, arguments[0]);

	if (arguments.size() == 1)
	{
		if (channel->getTopic() == "")
			return client->reply(RPL_NOTOPIC, channel->getName());
		else
		{
			client->reply(RPL_TOPIC, channel->getName(), channel->getTopic());
			return client->reply(RPL_TOPICWHOTIME, channel->getName(), channel->getTopicSetter(), toString(channel->getTopicTimestamp()));
		}
	}

	if (channel->isTopicProtected() && channel->getChannelUserMode(client) < OPERATOR)
		return client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());

	channel->setTopic(client, arguments[1]);
	broadcast(channel->getChannelUsers(), client->getPrefix() + " TOPIC " + channel->getName() + " :" + arguments[1]);
}