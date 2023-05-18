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
	/* target->reply("RPL_INVITING", arguments[0], arguments[1]); */ // lorenzo Je pense c'est pas bon mais t'es chiant alors je te laisse te debrouiller miao miao surement "INVITE" a la place de "RPL_INVITING" mais je sais pas c'est toi le boss des messages d'erreur
}
