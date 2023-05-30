#include "Server.hpp"

void Server::handlePrivateMessage(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 1)
		return client->reply(ERR_NORECIPIENT, "PRIVMSG");

	if (arguments.size() < 2)
		return client->reply(ERR_NOTEXTTOSEND);

	if (arguments[0][0] == '#' || arguments[0][0] == '&')
	{
		Channel* targetChannel = getChannel(arguments[0]);
		if (targetChannel == NULL)
			return client->reply(ERR_NOSUCHCHANNEL, arguments[0]);

		if (targetChannel->isOnChannel(client) == false)
			return client->reply(ERR_CANNOTSENDTOCHAN, arguments[0]);

		if (targetChannel->isUserBanned(client))
			return client->reply(ERR_CANNOTSENDTOCHAN, arguments[0]);

		return broadcast(targetChannel->getChannelUsers(), client->getPrefix() + " PRIVMSG " + targetChannel->getName() + " :" + arguments[1], client);
	}

	Client* targetClient = getClient(arguments[0]);
	if (targetClient == NULL)
		return client->reply(ERR_NOSUCHNICK, arguments[0]);

	targetClient->reply(client->getPrefix() + " PRIVMSG " + targetClient->getNickname() + " :" + arguments[1]);
}