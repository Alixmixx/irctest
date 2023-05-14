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
	if (nickname.find_first_of(" ,?!@\0") != std::string::npos)
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
		sendErrorMessage(client, "ERR_NONICKNAMEGIVEN");
		return;
	}
	if (isNicknameValid(arguments[0]))
	{

		if (isNicknameAlreadyTaken(this->getClients(), arguments[0]))
		{
			sendErrorMessage(client, "ERR_NICKNAMEINUSE");
			return;
		}
		if (DEBUG)
			std::cout << "\n\nhandleNick OK | Socket client: " << client->getSocket() << " | Nick: " << arguments[0] << std::endl;
		client->setNickname(arguments[0]);
	}
	else
	{
		sendErrorMessage(client, "ERR_ERRONEUSNICKNAME");
		return;
	}
}