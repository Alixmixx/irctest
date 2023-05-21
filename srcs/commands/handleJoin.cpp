#include "Server.hpp"

static void newChannel(Client* client, std::string channelName, std::string channelPassword)
{
	if (client->getChannels().size() >= MAX_CHANNELS_PER_CLIENT)
		return client->reply(ERR_TOOMANYCHANNELS, channelName);

	Server*	 server = client->getServer();
	Channel* channel = new Channel(server, channelName);

	channel->setPassword(channelPassword);
	channel->addChannelUser(client, FOUNDER);
	server->addChannel(channel);
}

static void addToChannel(Client* client, Channel* channel, std::string channelPassword)
{
	if (client->getChannels().size() >= MAX_CHANNELS_PER_CLIENT)
		return client->reply(ERR_TOOMANYCHANNELS, channel->getName());

	if (channel->isPasswordProtected() && channel->getPassword() != channelPassword)
		return client->reply(ERR_BADCHANNELKEY, channel->getName());

	if (channel->getChannelUserMode(client) == BANNED)
		return client->reply(ERR_BANNEDFROMCHAN, channel->getName());

	if (channel->getChannelUsers().size() >= MAX_USERS_PER_CHANNEL)
		return client->reply(ERR_CHANNELISFULL, channel->getName());

	if (channel->isInviteOnly() && channel->getChannelUserMode(client) != INVITED)
		return client->reply(ERR_INVITEONLYCHAN, channel->getName());

	if (channel->isOnChannel(client))
		return;

	channel->addChannelUser(client);
}

static std::string extractFromArgument(std::string& arguments)
{
	std::string name;
	size_t		pos = arguments.find_first_of(',');
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

static bool checkChannelName(std::string& channelName)
{
	if (channelName[0] != '#' && channelName[0] != '&')
	{
		// client->reply(ERR_NOSUCHCHANNEL, channelName);
		return (false);
	}
	return (true);
}

void Server::handleJoin(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() == 0)
		return client->reply(ERR_NEEDMOREPARAMS, "JOIN");

	std::string channelName;
	std::string channelPassword;

	while (!arguments[0].empty()) // alix > a changer en split
	{
		channelName = extractFromArgument(arguments[0]);

		if (checkChannelName(channelName) == false)
		{
			client->reply(ERR_NOSUCHCHANNEL, channelName);
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

		Channel* channel = getChannel(channelName);
		if (channel != NULL)
		{
			addToChannel(client, channel, channelPassword);
			if (channel->getTopic() == "")
				client->reply(RPL_NOTOPIC, channel->getName());
			else
				client->reply(RPL_TOPIC, channel->getName(), channel->getTopic());
		}
		else
		{
			newChannel(client, channelName, channelPassword);
		}
	}
}