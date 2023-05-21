#include "Server.hpp"

void Server::handleAdmin(Client* client, std::vector<std::string> arguments)
{
	(void)client;
	(void)arguments;
	client->reply(RPL_ADMINME, std::string("Administrative info about ") + SERVERHOSTNAME);
	client->reply(RPL_ADMINLOC1, "Ecole 42");
	client->reply(RPL_ADMINLOC2, "96 Boulevard Bessieres, 75017");
	client->reply(RPL_ADMINEMAIL, SERVERMAIL);
}
