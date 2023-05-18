#include "Server.hpp"

void Server::handlePrivateMessage(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
	{
		client->reply(ERR_NORECIPIENT, "PRIVMSG");
		return;
	}

	if (arguments.size() < 2)
	{
		client->reply(ERR_NOTEXTTOSEND);
		return;
	}

	if (arguments[0][0] == '#' || arguments[0][0] == '&')
	{
		// get channel
		Channel *targetChannel = getChannel(arguments[0]);
		if (targetChannel == NULL)
		{
			client->reply(ERR_NOSUCHCHANNEL, arguments[0]);
			return;
		}

		// check if client has modes to send message
		if (targetChannel->isOnChannel(client) == false)
		{
			client->reply(ERR_CANNOTSENDTOCHAN, arguments[0]);
			return;
		}

		// send message to all clients in channel

		broadcast(targetChannel->getChannelUsers(), ":" + client->getNickname() + " PRIVMSG " + arguments[0] + " " + concatenateArguments(arguments, 1), client);
		return;
	}

	Client *targetClient = getClient(arguments[0]);
	if (targetClient == NULL)
	{
		// get channel
		client->reply(ERR_NOSUCHNICK, arguments[0]);
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

	// TODO targetClient->reply("PRIVMSG", client->getNickname(), message);
}