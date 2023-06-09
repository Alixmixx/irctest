#include "Server.hpp"

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

void Server::handleNick(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty() || arguments[0].empty())
		return client->reply(ERR_NONICKNAMEGIVEN);

	if (!isNicknameValid(arguments[0]))
	{
		if (!client->isRegistered())
			client->setNickname("*");
		return client->reply(ERR_ERRONEUSNICKNAME, arguments[0]);
	}

	unsigned int suffix = 0;
	std::string	 nickname = arguments[0];
	while (getClient(nickname) != NULL)
	{
		if (client->isRegistered())
			return client->reply(ERR_NICKNAMEINUSE, nickname);
		nickname = arguments[0] + toString(suffix++);
	}
	if (client->isRegistered())
		broadcast(_clients, client->getPrefix() + " NICK " + nickname);
	else if (suffix)
		client->reply(":" + arguments[0] + " NICK " + nickname);
	client->setNickname(nickname);

	if (!client->isRegistered() && client->getUsername() != "")
		welcomeMessage(client);
}