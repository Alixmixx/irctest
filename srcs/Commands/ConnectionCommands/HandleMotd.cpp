#include "Server.hpp"

void Server::handleMotd(Client *client, std::vector<std::string> arguments)
{
	if (arguments.size() > 0)  // TODO VERIFIER SI TOUT MARCHE
	{
		if (arguments[0] != _serverName)
		{
			replyMessage(client, "ERR_NOSUCHSERVER");
		}
	}
	if (_serverMotd == "")
	{
		replyMessage(client, "ERR_NOMOTD");
	}
	replyMessage(client, "RPL_MOTDSTART");
	replyMessage(client, "RPL_MOTD");
	replyMessage(client, "RPL_ENDOFMOTD");
}