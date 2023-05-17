#include "Server.hpp"

static void newChannel(Client *client, std::string channelName, std::string channelPassword)
{
	if (client->getChannelCount() >= MAX_CHANNELS_PER_CLIENT)
	{
		client->reply("ERR_TOOMANYCHANNELS", channelName);
		return;
	}

	Server *server = client->getServer();
	Channel *channel = new Channel(server, channelName);

	channel->setPassword(channelPassword);
	channel->addChannelUser(client, FOUNDER);
	client->setChannelCount(client->getChannelCount() + 1);
	server->addChannel(channel);
}

static void addToChannel(Client *client, Channel *channel, std::string channelPassword)
{
	if (client->getChannelCount() >= MAX_CHANNELS_PER_CLIENT)
	{
		client->reply("ERR_TOOMANYCHANNELS", channel->getName());
		return;
	}

	if (channel->isPasswordProtected() && channel->getPassword() != channelPassword)
	{
		client->reply("ERR_BADCHANNELKEY", channel->getName());
		return;
	}

	if (channel->getChannelUserMode(client) == BANNED)
	{
		client->reply("ERR_BANNEDFROMCHAN", channel->getName());
		return;
	}

	if (channel->getChannelUsers().size() >= MAX_USERS_PER_CHANNEL)
	{
		client->reply("ERR_CHANNELISFULL", channel->getName());
		return;
	}

	if (channel->isInviteOnly() && channel->getChannelUserMode(client) != INVITED)
	{
		client->reply("ERR_INVITEONLYCHAN", channel->getName());
		return;
	}

	if (channel->isOnChannel(client))
	{
		return;
	}

	channel->addChannelUser(client);
	client->setChannelCount(client->getChannelCount() + 1);
}

static std::string extractFromArgument(std::string &arguments)
{
	std::string name;
	size_t pos = arguments.find_first_of(',');
	if (pos == std::string::npos)
	{
		name = arguments;
		arguments = "";
	}
	else
	{
		name = arguments.substr(0, pos);
		arguments = arguments.substr(pos + 1);
	}
	return (name);
}

static bool checkChannelName(std::string &channelName)
{
	if (channelName[0] != '#' && channelName[0] != '&')
	{
		// client->reply("ERR_NOSUCHCHANNEL", channelName);
		return (false);
	}
	return (true);
}

void Server::handleJoin(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() == 0)
	{
		client->reply("ERR_NEEDMOREPARAMS", "JOIN");
		return;
	}

	std::string channelName;
	std::string channelPassword;

	while (!arguments[0].empty()) // alix > a changer en split
	{
		channelName = extractFromArgument(arguments[0]);

		if (checkChannelName(channelName) == false)
		{
			client->reply("ERR_NOSUCHCHANNEL", channelName);
			return; // maybe continue ?
		}

		if (arguments.size() > 1)
		{
			channelPassword = extractFromArgument(arguments[1]);
		}
		else
		{
			channelPassword = "";
		}
		
		Channel *channel = getChannel(channelName);
		if (channel != NULL)
		{
			addToChannel(client, channel, channelPassword);
		}
		else
		{
			newChannel(client, channelName, channelPassword);
		}
	}
}