#include "Server.hpp"

static void welcomeMessage(Client *client)
{
	Server *server = client->getServer();

	server->replyMessage(client, "RPL_WELCOME");
	server->replyMessage(client, "RPL_YOURHOST");
	server->replyMessage(client, "RPL_CREATED");
	server->replyMessage(client, "RPL_MYINFO");
}

void Server::handleUser(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty() || arguments.size() < 4)
	{
		replyMessage(client, "ERR_NEEDMOREPARAMS", "USER");
		return;
	}
	if (client->getIsRegistered())
	{
		replyMessage(client, "ERR_ALREADYREGISTERED");
		return;
	}

	client->setUsername(arguments[0]);
	// arguments[1] j'ai pas capté a quoi ca sert, a investiguer
	client->setHostname(arguments[2]);
	client->setRealname(arguments[3]); // attention real name peut contenir des espaces, donc faut faire un iterateur tant que y'a pas de : et ajouter les arguments dans le realname

	if (DEBUG)
	{
		std::cout << "Nickname: " << client->getNickname() << " "
				  << "Username: " << client->getUsername() << " "
				  << "Hostname: " << client->getHostname() << " "
				  << "Realname: " << client->getRealname() << std::endl;
	}

	if (client->getUsername() != "" && client->getNickname() != "")
	{
		client->setIsRegistered(true);
		welcomeMessage(client);
	}
}
