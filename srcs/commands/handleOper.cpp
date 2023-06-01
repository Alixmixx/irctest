#include "Server.hpp"

void Server::handleOper(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() < 2)
		return client->reply(ERR_NEEDMOREPARAMS, "OPER");

	Client *oper = getClient(arguments[0]);
	if (client != oper)
		return client->reply(ERR_NOOPERHOST);

	if (arguments[1] != IRCOPPASSWORD)
		return client->reply(ERR_PASSWDMISMATCH);

	client->setAdmin(true);
	client->reply("MODE " + client->getNickname() + " +o"); //TODO: check if this is the right way to do it
	client->reply(RPL_YOUREOPER);
}