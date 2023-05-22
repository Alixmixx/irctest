#include "Server.hpp"

// TODO everything. I'm just pushing to merge
void Server::handlePass(Client* client, std::vector<std::string> arguments)
{
	if (client->isRegistered())
		return client->reply(ERR_ALREADYREGISTRED);
	if (arguments.empty())
		return client->reply(ERR_NEEDMOREPARAMS, "PASS");
	if (arguments[0] != PASSWORD)
		return client->reply(ERR_PASSWDMISMATCH);
	client->setPasswordCorrect();
}
