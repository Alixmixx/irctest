#include "Server.hpp"

void Server::handlePart(Client *client, std::vector<std::string> argument)
{
	if (argument.size() < 1)
	{
		client->reply("ERR_NEEDMOREPARAMS", "PART");
		return ;
	}

	std::string reason = "";
	if (argument.size() > 1)
		reason = " " + concatenateArguments(argument, 1);

	std::vector<std::string> channels = split(argument[0], ',');
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = getChannel(*it);
		if (channel == NULL)
		{
			client->reply("ERR_NOSUCHCHANNEL", channel->getName());
			continue;
		}
		if (channel->isOnChannel(client) == false)
		{
			client->reply("ERR_NOTONCHANNEL", channel->getName());
			continue;
		}
		broadcast(channel->getChannelUsers(), client->getNickname() + " PART " + channel->getName() + reason); //lorenzo miaoo PART #test
		channel->removeChannelUser(client);
		if (channel->getChannelUsers().size() == 0)
			removeChannel(channel);
	}

}
