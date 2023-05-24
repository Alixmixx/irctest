#include "Server.hpp"

static int countInvisibleUsers(Server* server)
{
	int					 n = 0;
	std::vector<Client*> clients = server->getClients();
	for (std::vector<Client*>::const_iterator client = clients.begin(); client != clients.end();
		 ++client)
		n += (*client)->isInvisible();
	return n;
}

static int countUnknownConnections(Server* server)
{
	int					 n = 0;
	std::vector<Client*> clients = server->getClients();
	for (std::vector<Client*>::const_iterator client = clients.begin(); client != clients.end();
		 ++client)
		n += !(*client)->isRegistered();
	return n;
}

void Server::handleLusers(Client* client, std::vector<std::string> arguments)
{
	const std::string currentUsers = toString(_clients.size());
	const std::string maxUsers = toString(_maxUsers);
	const std::string invisibleUsers = toString(countInvisibleUsers(this));
	const std::string unknownConnections = toString(countUnknownConnections(this));
	(void)arguments;
	client->reply(RPL_LUSERCLIENT, currentUsers, invisibleUsers, "1");
	client->reply(RPL_LUSEROP, "0");
	client->reply(RPL_LUSERUNKNOWN, unknownConnections);
	client->reply(RPL_LUSERCHANNELS, toString(_channels.size()));
	client->reply(RPL_LUSERME, currentUsers, "1");
	client->reply(RPL_LOCALUSERS, currentUsers, maxUsers);
	client->reply(RPL_GLOBALUSERS, currentUsers, maxUsers);
}
