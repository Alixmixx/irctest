#include "Server.hpp"

static std::vector<Client *> getRecipients(Client *client)
{
	std::set<std::string> nicknames;
	std::vector<Client *> recipients;
	std::vector<Channel *> channels = client->getChannels();
	for (std::vector<Channel *>::iterator channel = channels.begin(); channel != channels.end(); ++channel)
	{
		std::vector<Client *> users = (*channel)->getChannelUsers();
		for (std::vector<Client *>::iterator user = users.begin(); user != users.end(); ++user)
		{
			std::string nickname = (*user)->getNickname();
			if (*user != client && nicknames.find(nickname) == nicknames.end())
			{
				nicknames.insert(nickname);
				recipients.push_back(*user);
			}
		}
	}
	return recipients;
}

void Server::handleQuit(Client *client, std::vector<std::string> arguments)
{
	std::string message = arguments.empty() ? "" : "Quit: " + arguments[0];
	if (arguments.size() > 2 && arguments[2] == KILL)
		message = "Killed by " + arguments[0] + " : " + arguments[1];

	std::string quitMessage = client->getPrefix() + " QUIT :" + message;
	if (client->isRegistered())
	{
		FormerClient *formerClient = new FormerClient(client->getNickname(), client->getUsername(), client->getRealname(), client->getHostname(), std::time(NULL));
		_formerClients.push_back(formerClient);
		std::vector<Client *> recipients = getRecipients(client);
		broadcast(recipients, quitMessage);
	}
	removeClient(client);
}