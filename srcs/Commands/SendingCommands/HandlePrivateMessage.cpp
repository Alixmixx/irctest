#include "Server.hpp"

void Server::handlePrivateMessage(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
	{
		client->reply("ERR_NORECIPIENT", "PRIVMSG");
		return;
	}

	if (arguments.size() < 2)
	{
		client->reply("ERR_NOTEXTTOSEND");
		return;
	}

	Client *targetClient = getClient(arguments[0]);
	if (targetClient == NULL)
	{
		// get channel
		client->reply("ERR_NOSUCHNICK", arguments[0]);
		return;
	}

	std::string message = "";
	for (std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); ++it)
	{
		message += *it;
		if (it != arguments.end() - 1)
		{
			message += " ";
		}
	}

	targetClient->reply("PRIVMSG", client->getNickname(), message);
}