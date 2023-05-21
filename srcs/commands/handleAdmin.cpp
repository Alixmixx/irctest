#include "Server.hpp"

void Server::handleAdmin(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() >= 1 && arguments[0] != SERVERHOSTNAME)
		return client->reply(ERR_NOSUCHSERVER, arguments[0]);
	client->reply(RPL_ADMINME, std::string("Administrative info about ") + SERVERHOSTNAME);
	client->reply(RPL_ADMINLOC1, SERVERLOC1);
	client->reply(RPL_ADMINLOC2, SERVERLOC2);
	client->reply(RPL_ADMINEMAIL, SERVERMAIL);
}
