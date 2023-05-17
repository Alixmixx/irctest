#include "Server.hpp"

static void showChannelUsers(Channel *channel, Client *client, bool showInvisible)
{
	std::string symbol = "=";
	std::string names = "";

	if (channel->isSecret() == true)
		symbol = "@";
	else if (channel->isInviteOnly() == true)
		symbol = "*";

	(void)showInvisible;
	// for (std::vector<Client *>::iterator it = channel->getChannelUsers().begin(); it != channel->getChannelUsers().end(); it++)
	// {
	// 	if ((*it)->isInvisible() == false || showInvisible == true)
	// 	{
	// 		if (channel->isOperator(*it) == true)
	// 			names += "@";
	// 		else
	// 			names += "=";

	// 		names += (*it)->getNickname() + " ";
	// 	}
	// }

	client->reply("RPL_NAMREPLY", names);
	client->reply("RPL_ENDOFNAMES", channel->getName());
}

void Server::handleNames(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
	{
		client->reply("ERR_NEEDMOREPARAMS", "NAMES");
		return ;
	}

	if (arguments[0].find(',') != std::string::npos)
	{
		client->reply("RPL_ENDOFNAMES", "TARGMAX 1");
		return ;
	}

	Channel *channel = getChannel(arguments[0]);
	if (channel == NULL || channel->getChannelUsers().size() == 0)
	{
		client->reply("RPL_ENDOFNAMES", arguments[0]);
		return ;
	}

	// if (channel->isOnChannel(client) == false && (channel->isSecret() == true))
	// {
	// 	client->reply("RPL_ENDOFNAMES", arguments[0]);
	// 	return ;
	// }

	if (channel->isOnChannel(client) == false) // Hide invisible users
	{
		showChannelUsers(channel, client, false);
		return ;
	}

	if (channel->isOnChannel(client) == true) // Show invisible users
	{
		showChannelUsers(channel, client, true);
		return ;
	}
}