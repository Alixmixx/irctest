#include "Server.hpp"

void Server::handlePass(Client* client, std::vector<std::string> arguments)
{
	if (client->isPasswordCorrect())
		return client->reply(ERR_ALREADYREGISTRED);
	if (arguments.empty())
		return client->reply(ERR_NEEDMOREPARAMS, "PASS");
	if (arguments[0] != _serverPassword)
	{
		client->reply(ERR_PASSWDMISMATCH);
		client->reply("ERROR :Invalid password");
		client->_message = "";
		return removeClient(client);
	}
	client->setPasswordCorrect();
}
