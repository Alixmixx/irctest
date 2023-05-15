#include "Server.hpp"

void Server::handlePrivateMessage(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
	{
		replyMessage(client, "ERR_NORECIPIENT", "PRIVMSG");
		return;
	}
	if (arguments.size() < 2)
	{
		replyMessage(client, "ERR_NOTEXTTOSEND");
		return;
	}

	Client *targetClient = getClient(arguments[0]);
	if (targetClient == NULL)
	{
		// get channel
		replyMessage(client, "ERR_NOSUCHNICK", targetClient->getNickname());
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

	replyMessage(targetClient, "PRIVMSG", client->getNickname(), message);
}