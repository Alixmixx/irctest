#include "Server.hpp"

void Server::handleAdmin(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() >= 1)
	{
		Client *target = getClient(arguments[0]);
		if (!target && toLowerCase(arguments[0]) != toLowerCase(SERVERHOSTNAME) && toLowerCase(arguments[0]) != toLowerCase(SERVERNAME))
			return client->reply(ERR_NOSUCHSERVER, arguments[0]);
	}
	client->reply(RPL_ADMINME, std::string("Administrative info about ") + SERVERHOSTNAME);
	client->reply(RPL_ADMINLOC1, SERVERLOC1);
	client->reply(RPL_ADMINLOC2, SERVERLOC2);
	client->reply(RPL_ADMINEMAIL, SERVERMAIL);
}
