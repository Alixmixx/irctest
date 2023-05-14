#include "Server.hpp"

void Server::handlePrivateMessage(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
	{
		replyMessage(client, "ERR_NORECIPIENT", "PRIVMSG");
		return;
	}
	if (arguments.size() < 3)
	{
		replyMessage(client, "ERR_NOTEXTTOSEND");
		return;
	}

	Client *targetClient = getClient(arguments[0]);
	if (targetClient == NULL)
	{
		//get channel
		replyMessage(client, "ERR_NOSUCHNICK", targetNickname);
		return;
	}

	replyMessage(targetClient, "PRIVMSG", client->getNickname(), arguments[1]);
}