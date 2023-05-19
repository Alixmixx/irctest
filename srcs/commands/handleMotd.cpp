#include "Server.hpp"

void Server::handleMotd(Client* client, std::vector<std::string> arguments)
{
	(void)client;
	if (arguments.size() > 0) // TODO VERIFIER SI TOUT MARCHE
	{
		if (arguments[0] != _serverName)
		{
			/* 	client->reply(ERR_NOSUCHSERVER); */
		}
	}
	if (_serverMotd == "")
	{
		/* 		client->reply(ERR_NOMOTD); */
	}
	/* 	client->reply(RPL_MOTDSTART);
		client->reply(RPL_MOTD);
		client->reply(RPL_ENDOFMOTD); */
}