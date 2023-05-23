#include "Server.hpp"

/*
WHO #chan (in chan and not)
WHO axbrisse2
WHO mask*obordel

:liquid.oftc.net 352 axbrisse1 #lololol ~axbrisse 62.210.34.81 liquid.oftc.net axbrisse1 H :0 Axel BRISSE
:liquid.oftc.net 352 axbrisse1 #lololol ~axbrisse 62.210.34.81 liquid.oftc.net axbrisse2 H@ :0 Axel BRISSE
:liquid.oftc.net 315 axbrisse1 #lololol :End of /WHO list.

352 WHOREPLY
315 ENDOFWHO
402 NOSUCHSERVER
*/

void Server::handleWho(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
		return client->reply(ERR_NEEDMOREPARAMS, "WHO");
	std::string name = arguments[0];
	if (name[0] == '#' || name[0] == '&')
	{
		Channel* channel = getChannel(name);
		if (channel != NULL && channel->isOnChannel(client))
		{
			std::vector<Client*> clients = channel->getChannelUsers();
			for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				client->reply(RPL_WHOREPLY,
							  name + " " + client->getUsername() + " " + client->getIp() + " " +
								  SERVERHOSTNAME + " " + client->getNickname() + " H",
							  client->getRealname());
			}
		}
	}
	else
	{
	}
	client->reply(RPL_ENDOFWHO, name);
}
