#include "Server.hpp"

void Server::handleInvite(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS, "INVITE");
		return;
	}

	Client *target = getClient(arguments[0]);
	if (target == NULL)
	{
		client->reply(ERR_NOSUCHNICK, arguments[0]);
		return;
	}

	Channel *channel = getChannel(arguments[1]);
	if (channel == NULL)
	{
		client->reply(ERR_NOSUCHCHANNEL, arguments[1]);
		return;
	}

	if (channel->isOnChannel(client) == false)
	{
		client->reply(ERR_NOTONCHANNEL, client->getNickname());
		return;
	}

	if (channel->isInviteOnly() == true && channel->getChannelUserMode(client) < OPERATOR)
	{
		client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());
		return;
	}

	if (channel->isOnChannel(target) == true)
	{
		client->reply(ERR_USERONCHANNEL, arguments[0]);
		return;
	}

	channel->setClientMode(target, INVITED);
	client->reply(RPL_INVITING, arguments[0], arguments[1]);
	target->reply(client->getPrefix() + " INVITE " + client->getNickname() + " " + arguments[1]);
}
