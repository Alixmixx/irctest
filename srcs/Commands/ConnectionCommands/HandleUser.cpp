#include "Server.hpp"

static void welcomeMessage(Client *client)
{
	client->reply("RPL_WELCOME");
	client->reply("RPL_YOURHOST");
	client->reply("RPL_CREATED");
	client->reply("RPL_MYINFO");
}

void Server::handleUser(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty() || arguments.size() < 4)
	{
		client->reply("ERR_NEEDMOREPARAMS", "USER");
		return;
	}
	if (client->getIsRegistered())
	{
		client->reply("ERR_ALREADYREGISTERED");
		return;
	}

	client->setUsername(arguments[0]);
	// arguments[1] j'ai pas capté a quoi ca sert, a investiguer TODO
	client->setHostname(arguments[2]);
	client->setRealname(arguments[3]); // TODO attention real name peut contenir des espaces, donc faut faire un iterateur tant que y'a pas de : et ajouter les arguments dans le realname

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
