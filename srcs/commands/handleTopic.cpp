#include "Server.hpp"

void Server::handleTopic(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
	{
		client->reply(ERR_NEEDMOREPARAMS, "TOPIC");
		return;
	}

	// Handle TOPIC error cases
	Channel* channel = getChannel(arguments[0]);
	if (channel == NULL)
	{
		client->reply(ERR_NOSUCHCHANNEL, arguments[0]);
		return;
	}

	if (!channel->isOnChannel(client))
	{
		client->reply(ERR_NOTONCHANNEL, channel->getName());
		return;
	}

	// Handle TOPIC <channel>
	if (arguments.size() == 1)
	{
		if (channel->getTopic() == "")
		{
			client->reply(RPL_NOTOPIC, channel->getName());
			return;
		}

		//	client->reply(RPL_TOPIC, channel->getName(), channel->getTopic());
		//	client->reply(RPL_TOPICWHOTIME, channel->getName(), channel->getTopicSetter(), channel->getTopicTimestamp());
		return;
	}

	// Handle TOPIC <channel> <topic>
	std::string topic = concatenateArguments(arguments, 1);

	if (channel->getChannelUserMode(client) < OPERATOR) // protected topic ?
	{
		client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());
		return;
	}

	channel->setTopic(client, topic);
	broadcast(channel->getChannelUsers(), client->getNickname() + " TOPIC " + channel->getName() + " :" + topic); // Probably false
}