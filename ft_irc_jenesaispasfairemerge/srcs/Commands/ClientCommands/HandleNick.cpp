#include "Server.hpp"

static std::string toLowercase(const std::string &str)
{
	std::string lowercaseStr = str;
	for (std::string::iterator it = lowercaseStr.begin(); it != lowercaseStr.end(); ++it)
	{
		*it = std::tolower(*it);
	}
	return lowercaseStr;
}

static bool isNicknameValid(std::string nickname)
{
	if (nickname.length() > 9)
		return false;
	if (nickname.find_first_of(" ,?!@") != std::string::npos)
		return false;
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#' || nickname[0] == '&')
		return false;
	return true;
}

static bool isNicknameAlreadyTaken(Server server, std::string nickname)
{
	if (server.getClients().size() == 1)
		return false;
	std::vector<Client *>::const_iterator it = server.getClients().begin();
	for (; it != server.getClients().end(); ++it)
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

		if (isNicknameAlreadyTaken(*this, arguments[0]))
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