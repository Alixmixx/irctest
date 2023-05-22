#include "Server.hpp"

void Server::handleInvite(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
		return client->reply(ERR_NEEDMOREPARAMS, "INVITE");


	Client* target = getClient(arguments[0]);

	if (target == NULL)
		return client->reply(ERR_NOSUCHNICK, arguments[0]); // TODO est-ce que c'est le bon ordre


	Channel* channel = getChannel(arguments[1]);

	if (channel == NULL)
		return client->reply(ERR_NOSUCHCHANNEL, arguments[1]);

	if (channel->isOnChannel(client) == false)
		return client->reply(ERR_NOTONCHANNEL, arguments[1]);

	if (channel->isInviteOnly() == true && channel->getChannelUserMode(client) < OPERATOR)
		return client->reply(ERR_CHANOPRIVSNEEDED, arguments[1]);

	if (channel->isOnChannel(target) == true)
		return client->reply(ERR_USERONCHANNEL, arguments[0], arguments[1]);


	channel->setClientMode(target, INVITED);
	client->reply(RPL_INVITING, arguments[0], arguments[1]);
	target->reply(client->getPrefix() + " INVITE " + client->getNickname() + " " + channel->getName()); // voir si le # est ajoute tout seul
}
