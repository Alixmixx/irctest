#include "Server.hpp"

void Server::handleKick(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
		return client->reply(ERR_NEEDMOREPARAMS, "KICK");

	Channel* channel = getChannel(arguments[0]);

	if (channel == NULL)
		return client->reply(ERR_NOSUCHCHANNEL, arguments[0]);

	if (channel->isOnChannel(client) == false)
		return client->reply(ERR_NOTONCHANNEL, arguments[0]);

	if (channel->getChannelUserMode(client) < MODERATOR)
		return client->reply(ERR_CHANOPRIVSNEEDED, arguments[0]);

	if (arguments[1].find(',') != std::string::npos)
		arguments[1] = arguments[1].substr(0, arguments[1].find(','));

	Client* target = getClient(arguments[1]);

	if (target == NULL || channel->isOnChannel(target) == false)
		return client->reply(ERR_USERNOTINCHANNEL, arguments[1], arguments[0]);

	if (channel->getChannelUserMode(client) < channel->getChannelUserMode(target))
		return client->reply(ERR_CHANOPRIVSNEEDED, arguments[0]);

	broadcast(channel->getChannelUsers(), client->getPrefix() + " KICK " + channel->getName() +
											  " " + target->getNickname() +
											  (arguments[2] == ":" ? "" : " :" + arguments[2]));
	channel->removeClientFromChannel(target); // not sure
	channel->setClientMode(target, NOTINCHANNEL);
}