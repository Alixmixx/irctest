#include "Server.hpp"

void Server::handleKick(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS, "KICK");
		return;
	}

	if (arguments[0].find(',') != std::string::npos)
	{
		client->reply(ERR_BADCHANMASK, arguments[0]);
		return;
	}

	Channel* channel = getChannel(arguments[0]);
	if (channel == NULL)
	{
		client->reply(ERR_NOSUCHCHANNEL, arguments[0]);
		return;
	}

	if (channel->isOnChannel(client) == false)
	{
		client->reply(ERR_NOTONCHANNEL, arguments[0]);
		return;
	}

	if (channel->getChannelUserMode(client) < MODERATOR)
	{
		client->reply(ERR_CHANOPRIVSNEEDED, arguments[0]);
		return;
	}

	Client* target = getClient(arguments[1]);

	if (target == NULL)
	{
		client->reply(ERR_NOSUCHNICK, arguments[1]);
		return;
	}

	if (channel->isOnChannel(target) == false)
	{
		client->reply(ERR_USERNOTINCHANNEL, arguments[1], arguments[0]);
		return;
	}

	if (channel->getChannelUserMode(client) < channel->getChannelUserMode(target))
	{
		client->reply(ERR_CHANOPRIVSNEEDED, arguments[0]);
		return;
	}

	std::string reason = concatenateArguments(arguments, 2);
	broadcast(channel->getChannelUsers(), client->getPrefix() + " KICK " + arguments[0] + " " + arguments[1] + (reason == ":" ? "" : " " + reason));
	channel->removeClientFromChannel(target);
	channel->setClientMode(target, BANNED);
	// MAYBE ADD A REPLY TO THE TARGET
}