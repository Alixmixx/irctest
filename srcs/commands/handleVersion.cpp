#include "Server.hpp"

void Server::handleVersion(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() >= 1 && toLowerCase(arguments[0]) != toLowerCase(SERVERHOSTNAME) && toLowerCase(arguments[0]) != toLowerCase(SERVERNAME))
		return client->reply(ERR_NOSUCHSERVER, arguments[0]);
	client->reply(RPL_VERSION, SERVERVERSION, SERVERHOSTNAME, SERVERNAME);
}