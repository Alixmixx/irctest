#include "Server.hpp"

// TODO ERR_UMODEUNKNOWNFLAG for user modes

void Server::setModeClient(Client* client, std::vector<std::string> arguments)
{
	std::string replyString = "";
	std::string	 modeString = arguments[1];
	bool		 sign = PLUS;
	bool		 stringSign = PLUS;
	short		 addToString = 0;
	int			 first = 0;

	for (size_t i = 0; i < modeString.length(); i++)
	{
		addToString = 0;
		switch (modeString[i])
		{
		case '+':
			sign = PLUS;
			break;
		case '-':
			sign = MINUS;
			break;
		case 'i':
			if (!(first & M_INVISIBLE) && (first |= M_INVISIBLE) && ++addToString) (sign == PLUS) ? client->setIsInvisible(true) : client->setIsInvisible(false);
			break;
		default:
			if (!(first & M_ERROR) && (first |= M_ERROR)) client->reply(ERR_UNKNOWNMODE, std::string(1, modeString[i]));
			break;
		}
		if (addToString)
		{
			if ((sign == PLUS && stringSign == MINUS)
				|| (sign == MINUS && stringSign == PLUS)
				|| (replyString.empty()))
			{
				stringSign = sign;
				replyString += (sign == PLUS) ? '+' : '-';
			}
			replyString += modeString[i];
		}
	}
	if (replyString != "")
		client->reply(client->getPrefix() + " MODE " + client->getNickname() + " " + replyString);
}

void Server::setModeChannel(Client* client, Channel* channel, std::vector<std::string> arguments)
{
	std::string replyString = "";
	std::string	 modeString = arguments[1];
	std::string	 targets = "";
	bool		 sign = PLUS;
	bool		 stringSign = PLUS;
	short		 addToString = 0;
	int			 first = 0;
	unsigned int clientNumber = 2;

	for (size_t i = 0; i < modeString.length(); i++)
	{
		addToString = 0;
		switch (modeString[i])
		{
		case '+':
			sign = PLUS;
			break;
		case '-':
			sign = MINUS;
			break;
		case 'i':
			if (!(first & M_INVITE) && (first |= M_INVITE) && ++addToString) channel->setMode(M_INVITE, sign);
			break;
		case 'k':
			if (!(first & M_KEY) && (first |= M_KEY) && ++addToString) channel->setMode(M_KEY, sign);
			break;
		case 'l':
			if (!(first & M_LIMITED) && (first |= M_LIMITED) && ++addToString) channel->setMode(M_LIMITED, sign);
			break;
		case 'm':
			if (!(first & M_MODERATED) && (first |= M_MODERATED) && ++addToString) channel->setMode(M_MODERATED, sign);
			break;
		case 'o': {
			if (arguments.size() <= clientNumber)
			{
				addToString = 0;
				break;
			}
			Client* target = getClient(arguments[clientNumber]);
			if (target == NULL || !channel->isOnChannel(target))
			{
				addToString = 0;
				break;
			}
			if (channel->getChannelUserMode(client) <= channel->getChannelUserMode(target))
			{
				client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());
				addToString = 0;
				break;
			}
			if (sign == PLUS)
				channel->setClientMode(target, OPERATOR);
			else if (sign == MINUS)
				channel->setClientMode(target, USER);
			targets += " " + target->getNickname();
			addToString = 1;
			break;
		}
		case 's':
			if (!(first & M_SECRET) && (first |= M_SECRET) && ++addToString) channel->setMode(M_SECRET, sign);
			break;
		case 't':
			if (!(first & M_PROTECTED) && (first |= M_PROTECTED) && ++addToString) channel->setMode(M_PROTECTED, sign);
			break;
		default:
			if (!(first & M_ERROR) && (first |= M_ERROR)) client->reply(ERR_UNKNOWNMODE, std::string(1, modeString[i]));
			break;
		}
		if (addToString)
		{
			if ((sign == PLUS && stringSign == MINUS)
				|| (sign == MINUS && stringSign == PLUS)
				|| (replyString.empty()))
			{
				stringSign = sign;
				replyString += (sign == PLUS) ? '+' : '-';
			}
			replyString += modeString[i];
		}
	}
	if (replyString != "")
		broadcast(channel->getChannelUsers(), client->getPrefix() + " MODE " + channel->getName() + " " + replyString + targets);
}

void Server::handleMode(Client* client, std::vector<std::string> arguments) // TODO by Alix
{
	if (arguments.size() == 0)
		return client->reply(ERR_NEEDMOREPARAMS, "MODE");

	Client*	 target = getClient(arguments[0]);
	Channel* channel = getChannel(arguments[0]);

	if (target == NULL && arguments[0][0] != '#')
		return client->reply(ERR_NOSUCHNICK, arguments[0]);

	if (channel == NULL && arguments[0][0] == '#')
		return client->reply(ERR_NOSUCHCHANNEL, arguments[0]);

	if (target)
	{
		if (target != client)
			return client->reply(ERR_USERSDONTMATCH);

		if (arguments.size() == 1)
			return client->reply(RPL_UMODEIS, client->getModeString());

		return setModeClient(client, arguments);
	}
	if (channel)
	{
		if (channel->isOnChannel(client) == false && channel->isSecret() == true)
			return client->reply(ERR_NOSUCHCHANNEL, arguments[0]);

		if (arguments.size() == 1)
		{
			client->reply(RPL_CHANNELMODEIS, channel->getName(), channel->getModeString());
			return client->reply(RPL_CREATIONTIME, channel->getName(), toString(channel->getCreationTime()));
		}

		if (arguments.size() == 2 && arguments[1][0] == 'b')
			return client->reply(RPL_ENDOFBANLIST, channel->getName());

		if (channel->isOnChannel(client) == false || channel->getChannelUserMode(client) < OPERATOR)
			return client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());

		return setModeChannel(client, channel, arguments);
	}
}
