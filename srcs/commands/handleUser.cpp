#include "Server.hpp"

void Server::welcomeMessage(Client* client)
{
	client->setIsRegistered(true);
	client->setNickname(client->getNickname());
	client->reply(RPL_WELCOME, NETWORKNAME, client->getNickname(), client->getUsername(), client->getHostname());
	client->reply(RPL_YOURHOST, SERVERNAME, SERVERVERSION);
	return client->reply(RPL_CREATED, formatTime(_serverCreationTime));
}

void Server::handleUser(Client* client, std::vector<std::string> arguments)
{
	if (client->getNickname() == "*")
		return removeClient(client);

	if (arguments.size() < 4 || arguments[0] == "")
 		return client->reply(ERR_NEEDMOREPARAMS, "USER");

	if (client->isRegistered())
 		return client->reply(ERR_ALREADYREGISTRED);

	client->setUsername("~" + arguments[0]); // the username provided by the client SHOULD be prefixed by a tilde ('~', 0x7E) to show that this value is user-set.
	struct hostent *host = gethostbyname(inet_ntoa(client->getClientAddress().sin_addr));
	//if (host == NULL)
		// blablabla      à proteger probablement. kick le client?
	client->setHostname(host->h_name);
	client->setRealname(arguments[3]);
	std::cout << BLUE << *client << RESET << std::endl;
	if (client->getNickname() != "")
		welcomeMessage(client);
}
