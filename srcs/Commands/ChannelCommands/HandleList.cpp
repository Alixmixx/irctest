#include "Server.hpp"

void Server::handleList(Client *client, std::vector<std::string> arguments)
{
	client->reply("RPL_LISTSTART");

	if (arguments.size() == 0)
	{
		for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
		{
			Channel *channel = *it;
			if (channel->isSecret() == false)
				client->reply("RPL_LIST", channel->getName(), toString(channel->getChannelUsers().size()), channel->getTopic());
		}
		client->reply("RPL_LISTEND");
		return;
	}

	std::vector<std::string> channels = split(arguments[0], ',');

	// Maybe implement ELIST ?  if (arguments.size() > 1)

	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = getChannel(*it);
		if (channel == NULL)
		{
			client->reply("ERR_NOSUCHCHANNEL", channel->getName());
			continue;
		}
		if (channel->isSecret() == false)
			client->reply("RPL_LIST", channel->getName(), toString(channel->getChannelUsers().size()), channel->getTopic());
	}
	client->reply("RPL_LISTEND");
}
