#include "Server.hpp"

void Server::handleUser(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 4)
	{
		client->reply(ERR_NEEDMOREPARAMS, "USER");
		return;
	}
	if (client->IsRegistered())
	{
		client->reply(ERR_ALREADYREGISTRED);
		return;
	}

	client->setUsername(arguments[0]);
	// TODO investiguer arguments[1]
	client->setHostname(arguments[2]);
	client->setRealname(arguments[3]);
	std::cout << BLUE << *client << RESET << std::endl;
	if (client->getUsername() != "" && client->getNickname() != "")
	{
		client->setIsRegistered(true);
		client->setNickname(client->getNickname());
		client->reply(RPL_WELCOME, NETWORKNAME, client->getNickname(), client->getUsername(), client->getHostname());
		client->reply(RPL_YOURHOST, SERVERNAME, SERVERVERSION);
		client->reply(RPL_CREATED, formatTime(_serverCreationTime));
	}
	else
	{
		client->reply(ERR_ALREADYREGISTRED);
	}
}
