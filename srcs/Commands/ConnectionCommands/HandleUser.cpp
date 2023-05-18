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
	// arguments[1] j'ai pas capté a quoi ca sert, a investiguer TODO
	client->setHostname(arguments[2]);
	client->setRealname(arguments[3].substr(1)); // TODO attention real name peut contenir des espaces, donc faut faire un iterateur tant que y'a pas de : et ajouter les arguments dans le realname

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
		client->setNickname(client->getNickname());
		client->reply(RPL_WELCOME, NETWORKNAME, client->getNickname(), client->getUsername(), client->getHostname());
		client->reply(RPL_YOURHOST, SERVERNAME, SERVERVERSION);
		client->reply(RPL_CREATED, formatTime(_serverCreationTime));
		//client->reply(RPL_MYINFO); // TODO apres avoir lu la doc
		// 05 TODO pour axel
	}
	else
	{
		client->reply(ERR_ALREADYREGISTRED);
	}
}
