#include "Server.hpp"

void Server::handleUser(Client* client, std::vector<std::string> arguments)
{
	if (client->getNickname() == "*")
		return removeClient(client);

	if (arguments.size() < 4 || arguments[0] == "")
		return client->reply(ERR_NEEDMOREPARAMS, "USER");

	if (client->isRegistered())
		return client->reply(ERR_ALREADYREGISTRED);

	client->setUsername("~" + arguments[0]);
	char*			addr = inet_ntoa(client->getClientAddress().sin_addr);
	struct hostent* host = gethostbyname(addr);
	client->setHostname(host ? host->h_name : addr);
	client->setRealname(arguments[3]);
	if (client->getNickname() != "")
		welcomeMessage(client);
}
