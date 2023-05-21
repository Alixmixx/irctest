#include "Server.hpp"

void Server::handleAdmin(Client* client, std::vector<std::string> arguments)
{
	(void)arguments;
	client->reply(RPL_ADMINME, std::string("Administrative info about ") + SERVERHOSTNAME);
	client->reply(RPL_ADMINLOC1, SERVERLOC1);
	client->reply(RPL_ADMINLOC2, SERVERLOC2);
	client->reply(RPL_ADMINEMAIL, SERVERMAIL);
}
