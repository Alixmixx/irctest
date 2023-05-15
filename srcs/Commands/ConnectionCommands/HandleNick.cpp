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
	if (nickname.find_first_of(" ,?!@\0") != std::string::npos) // TODO tester si NULL character dans username fait planter le serveur
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
		replyMessage(client, "ERR_NONICKNAMEGIVEN");
		return;
	}
	if (isNicknameValid(arguments[0]))
	{
		if (isNicknameAlreadyTaken(this->getClients(), arguments[0]))
		{
			replyMessage(client, "ERR_NICKNAMEINUSE", arguments[0]);
			return;
		}
		if (!client->getNickname().empty())
		{
			replyMessage(client, "RPL_NICKCHANGE", client->getNickname(), arguments[0]);
		}
		client->setNickname(arguments[0]);
		// TODO broadcast nick change a tous les channels
		return;
	}
	else
	{
		replyMessage(client, "ERR_ERRONEUSNICKNAME", arguments[0]);
		return;
	}
}