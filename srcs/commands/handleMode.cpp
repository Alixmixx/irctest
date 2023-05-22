#include "Server.hpp"

// TODO ERR_UMODEUNKNOWNFLAG for user modes

void Server::setModeChannel(Client *client, Channel *channel, std::vector<std::string> arguments)
{
	// Find out if we're adding or removing modes iterate + 1
	std::string replyString = "";
	std::string modeString = arguments[1];
	bool sign = PLUS;
	bool addToString = false;
	int	first = 0;
	unsigned int clientNumber = 2;

	for (size_t i = 0; i < modeString.length(); i++)
	{
		addToString = true;
		switch (modeString[i])
		{
			case '+':
				sign = PLUS;
				break;
			case '-':
				sign = MINUS;
				break;
			case 'i':
				if (!(first & M_INVITE) && (first |= M_INVITE)) channel->setMode(M_INVITE, sign);
				break;
			case 'k':
				if (!(first & M_KEY) && (first |= M_KEY)) channel->setMode(M_KEY, sign);
				break;
			case 'l':
				if (!(first & M_LIMITED) && (first |= M_LIMITED)) channel->setMode(M_LIMITED, sign);
				break;
			case 'm':
				if (!(first & M_MODERATED) && (first |= M_MODERATED)) channel->setMode(M_MODERATED, sign);
				break;
			case 'o':
			{
				if (arguments.size() <= clientNumber)
				{
					addToString = false;
					break;
				}
				Client *target = getClient(arguments[clientNumber]);
				if (target == NULL || !channel->isOnChannel(target))
				{
					client->reply(ERR_USERNOTINCHANNEL, arguments[clientNumber]);
					addToString = false;
					break; 
				}
				if (sign == PLUS)
					channel->setClientMode(target, OPERATOR);
				else if (sign == MINUS)
					channel->setClientMode(target, USER);
				break;
			}
			case 's':
				if (!(first & M_SECRET) && (first |= M_SECRET)) channel->setMode(M_SECRET, sign);
				break;
			case 't':
				if (!(first & M_PROTECTED) && (first |= M_PROTECTED)) channel->setMode(M_PROTECTED, sign);
				break;
			default:
				addToString = false;
				break;
		}
		if (addToString)
			replyString += modeString[i];
	}
	broadcast(channel->getChannelUsers(), client->getPrefix() + " MODE " + channel->getName() + " " + replyString);
}

void Server::handleMode(Client* client, std::vector<std::string> arguments) // TODO by Alix
{
	if (arguments.size() == 0)
		return client->reply(ERR_NEEDMOREPARAMS, "MODE");

	Client *target = getClient(arguments[0]);
	Channel *channel = getChannel(arguments[0]);

	if (target == NULL && arguments[0][0] != '#')
		return client->reply(ERR_NOSUCHNICK, arguments[0]); // ?

	if (channel == NULL && arguments[0][0] == '#')
		return client->reply(ERR_NOSUCHCHANNEL, arguments[0]);

/* 	if (target)
	{
		if (target != client)
			return client->reply(ERR_USERSDONTMATCH);

		if (arguments.size() == 1)
			return client->reply(RPL_UMODEIS, target->getModeString());

		return setModeClient(client, arguments);
	} */
	if (channel)
	{
		if (channel->isOnChannel(client) == false && channel->isSecret() == true)
			return client->reply(ERR_NOSUCHCHANNEL, arguments[0]);

		if (arguments.size() == 1)
			return client->reply(RPL_CHANNELMODEIS, channel->getName(), channel->getModeString());
			// RPL_CREATIONTIME

		if (channel->isOnChannel(client) == false || channel->getChannelUserMode(client) < OPERATOR)
			return client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());

		return setModeChannel(client, channel, arguments);
	}
}
