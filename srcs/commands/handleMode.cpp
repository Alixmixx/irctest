#include "Server.hpp"

// TODO ERR_UMODEUNKNOWNFLAG for user modes

void Server::setModeClient(Client *client, std::vector<std::string> arguments)
{
	std::string replyString = "";
	std::string modeString = arguments[1];
	bool sign = PLUS;
	bool stringSign = PLUS;
	short addToString = 0;
	int first = 0;

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
			if (!(first & M_INVISIBLE) && (first |= M_INVISIBLE) && ++addToString)
				(sign == PLUS) ? client->setIsInvisible(true) : client->setIsInvisible(false);
			break;
		default:
			if (!(first & M_ERROR) && (first |= M_ERROR))
				client->reply(ERR_UNKNOWNMODE, std::string(1, modeString[i]));
			break;
		}
		if (addToString)
		{
			if ((sign == PLUS && stringSign == MINUS) || (sign == MINUS && stringSign == PLUS) || (replyString.empty()))
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

static std::string userMask(std::string input)
{
	std::string nickname;
	std::string username;
	std::string hostname;

	if (!(input.find("!") == std::string::npos && input.find("@") != std::string::npos))
		nickname = input.substr(0, input.find("!"));
	else
		username = input.substr(0, input.find("@"));

	if (nickname.empty())
		nickname = "*";

	if (input.find("!") != std::string::npos)
		username = input.substr(input.find("!") + 1, input.find("@") - input.find("!") - 1);

	if (username.empty())
		username = "*";

	if (input.find("@") != std::string::npos)
		hostname = input.substr(input.find("@") + 1);

	if (hostname.empty())
		hostname = "*";

	std::string mask = nickname + "!" + username + "@" + hostname;

	return mask;
}

void Server::setModeChannel(Client *client, Channel *channel, std::vector<std::string> arguments)
{
	std::string replyString = "";
	std::string modeString = arguments[1];
	std::string targets = "";
	bool sign = PLUS;
	bool stringSign = PLUS;
	short addToString = 0;
	int first = 0;
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
		case 'b':
		{
			if (arguments.size() <= clientNumber)
			{
				if (!(first & M_BAN) && (first |= M_BAN))
				{
					for (std::vector<std::string>::iterator it = channel->getBanList().begin(); it != channel->getBanList().end(); it++)
						client->reply(RPL_BANLIST, channel->getName(), *it);
					client->reply(RPL_ENDOFBANLIST, channel->getName());
				}
				break;
			}
			std::string mask = userMask(arguments[clientNumber++]);

			std::vector<std::string> &banList = channel->getBanList();
			if (sign == PLUS)
			{
				if (std::find(banList.begin(), banList.end(), mask) != banList.end())
					break;
				banList.push_back(mask);
			}
			else if (sign == MINUS)
				banList.erase(std::remove(banList.begin(), banList.end(), mask), banList.end());
			targets += " " + mask;
			addToString = 1;
			break;
		}
		case 'i':
			if (!(first & M_INVITE) && (first |= M_INVITE) && ++addToString)
				channel->setMode(M_INVITE, sign);
			break;
		case 'k':
		{
			if (!(first & M_KEY) && (first |= M_KEY))
			{
				std::string key;
				if (sign == MINUS)
					key = "";
				else if (arguments.size() <= clientNumber)
					break;
				else if (sign == PLUS)
				{
					key = arguments[clientNumber++];
					targets += " " + key;
				}
				channel->setPassword(key);
				channel->setMode(M_KEY, sign);
				addToString = 1;
			}
			break;
		}
		case 'l':
			if (!(first & M_LIMITED) && (first |= M_LIMITED))
			{
				if (sign == MINUS && ++addToString)
				{
					channel->setMode(M_LIMITED, sign);
					break;
				}
				int userLimit;
				if (arguments.size() <= clientNumber)
					userLimit = channel->getChannelUsers().size();
				else
				{
					std::string limit = arguments[clientNumber++];
					if ((limit.find_first_not_of("+0123456789") != std::string::npos || limit.length() > 11))
						break;

					userLimit = std::atoi(limit.c_str());

					if (userLimit < 1)
						break;
				}
				channel->setUserLimit(userLimit);
				channel->setMode(M_LIMITED, sign);
				addToString = 1;
				targets += " " + toString(userLimit);
			}
			break;
		case 'm':
			if (!(first & M_MODERATED) && (first |= M_MODERATED) && ++addToString)
				channel->setMode(M_MODERATED, sign);
			break;
		case 'o':
		{
			if (arguments.size() <= clientNumber)
				break;

			Client *target = getClient(arguments[clientNumber++]);
			if ((target == NULL || !channel->isOnChannel(target)))
				break;

			if (channel->getChannelUserMode(client) < OPERATOR) // channel->getChannelUserMode(target))
			{
				client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());
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
			if (!(first & M_SECRET) && (first |= M_SECRET) && ++addToString)
				channel->setMode(M_SECRET, sign);
			break;
		case 't':
			if (!(first & M_PROTECTED) && (first |= M_PROTECTED) && ++addToString)
				channel->setMode(M_PROTECTED, sign);
			break;
		case 'v':
		{
			if (arguments.size() <= clientNumber)
				break;

			Client *target = getClient(arguments[clientNumber++]);
			if (target == NULL || !channel->isOnChannel(target))
				break;

			if (channel->getChannelUserMode(client) < OPERATOR)//<= channel->getChannelUserMode(target))
			{
				client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());
				break;
			}

			if (sign == PLUS)
				channel->setClientMode(target, VOICE);
			else if (sign == MINUS)
				channel->setClientMode(target, USER);

			targets += " " + target->getNickname();
			addToString = 1;
			break;
		}
		default:
			if (!(first & M_ERROR) && (first |= M_ERROR))
				client->reply(ERR_UNKNOWNMODE, std::string(1, modeString[i]));
			break;
		}
		if (addToString)
		{
			if ((sign == PLUS && stringSign == MINUS) || (sign == MINUS && stringSign == PLUS) || (replyString.empty()))
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

void Server::handleMode(Client *client, std::vector<std::string> arguments) // TODO by Alix
{
	if (arguments.size() == 0)
		return client->reply(ERR_NEEDMOREPARAMS, "MODE");

	Client *target = getClient(arguments[0]);
	Channel *channel = getChannel(arguments[0]);

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

		if (arguments.size() == 2 && (arguments[1].find('b') != std::string::npos) && channel->getChannelUserMode(client) < OPERATOR)
			return client->reply(RPL_ENDOFBANLIST, channel->getName());

		if (channel->isOnChannel(client) == false || channel->getChannelUserMode(client) < OPERATOR)
			return client->reply(ERR_CHANOPRIVSNEEDED, channel->getName());

		return setModeChannel(client, channel, arguments);
	}
}
