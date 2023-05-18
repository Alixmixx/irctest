#include "Server.hpp"

static std::string toLowercase(std::string lowercaseStr)
{
	for (std::string::iterator it = lowercaseStr.begin(); it != lowercaseStr.end(); it++)
	{
		*it = std::tolower(*it);
	}
	return lowercaseStr;
}

static bool isNicknameValid(std::string nickname)
{
	if (nickname.length() > 9)
		return false;
	if (!isStringPrintable(nickname))
		return false;
	if (nickname.find_first_of(" ,?!@") != std::string::npos)
		return false;
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#' || nickname[0] == '&')
		return false;
	return true;
}

static bool isNicknameAlreadyTaken(std::vector<Client *> clients, const std::string &nickname)
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (toLowercase((*it)->getNickname()) == toLowercase(nickname))
			return true;
	}
	return false;
}

void Server::handleNick(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty() || arguments[0].empty())
	{
		client->reply(ERR_NONICKNAMEGIVEN);
		return;
	}
	if (isNicknameValid(arguments[0]))
	{
		unsigned int suffix = 1;
		std::string nickname = arguments[0];
		while (isNicknameAlreadyTaken(this->getClients(), nickname))
		{
			if (client->IsRegistered())
			{
				client->reply(ERR_NICKNAMEINUSE, nickname);
				return;
			}
			else
			{
				nickname = arguments[0] + toString(suffix++);
			}
		}
		if (client->IsRegistered())
		{
			broadcast(_clients, ":" + client->getNickname() + " NICK :" + nickname);
		}
		else
		{
			 client->reply(":" + arguments[0] + " NICK " + nickname);
		}
		client->setNickname(nickname);
		return;
	}
	else
	{
		client->reply(ERR_ERRONEUSNICKNAME, arguments[0]);
		return;
	}
}