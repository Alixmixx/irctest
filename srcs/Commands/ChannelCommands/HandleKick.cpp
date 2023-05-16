#include "Server.hpp"

void Server::handleKick(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		client->reply("ERR_NEEDMOREPARAMS", "KICK");
		return;
	}

	if (arguments[0].find(',') != std::string::npos)
	{
		client->reply("ERR_BADCHANMASK", arguments[0]);
		return;
	}

	if (isChannel(arguments[0]) == false)
	{
		client->reply("ERR_NOSUCHCHANNEL", arguments[0]);
		return;
	}

	Channel *channel = getChannel(arguments[0]);

	if (channel->isOnChannel(client) == false)
	{
		client->reply("ERR_NOTONCHANNEL", arguments[0]);
		return;
	}

	if (channel->isOperator(client) == false)
	{
		client->reply("ERR_CHANOPRIVSNEEDED", arguments[0]);
		return;
	}

	Client *target = getClient(arguments[1]);

	if (target == NULL)
	{
		client->reply("ERR_NOSUCHNICK", arguments[1]);
		return;
	}

	if (channel->isOnChannel(target) == false)
	{
		client->reply("ERR_USERNOTINCHANNEL", arguments[1]);
		return;
	}

	std::string reason = concatenateArguments(arguments, 2);
	broadcast(channel->getChannelUsers(), "KICK " + arguments[0] + " " + arguments[1] + " " + client->getNickname() + " " + (reason.empty() ? "" : " :" + reason));
	channel->removeClientFromChannel(target);
	channel->addChannelBan(target);
	// MAYBE ADD A REPLY TO THE TARGET
}