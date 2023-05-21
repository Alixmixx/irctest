#include "Server.hpp"

void Server::handleMotd(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() >= 1 && arguments[0] != SERVERHOSTNAME)
		return client->reply(ERR_NOSUCHSERVER, arguments[0]);
	client->reply(RPL_MOTDSTART, SERVERHOSTNAME);
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "                  '");
	client->reply(RPL_MOTD, "             *          .");
	client->reply(RPL_MOTD, "                    *       '");
	client->reply(RPL_MOTD, "               *                *");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "    *   '*");
	client->reply(RPL_MOTD, "            *");
	client->reply(RPL_MOTD, "                 *");
	client->reply(RPL_MOTD, "                        *");
	client->reply(RPL_MOTD, "                *");
	client->reply(RPL_MOTD, "                      *");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_MOTD, "");
	client->reply(RPL_ENDOFMOTD);
}