#include "Server.hpp"

/*
:liquid.oftc.net 352 axbrisse1 #lololol ~axbrisse 62.210.34.81 liquid.oftc.net axbrisse1 H :0 Axel BRISSE
:liquid.oftc.net 352 axbrisse1 #lololol ~axbrisse 62.210.34.81 liquid.oftc.net axbrisse2 H@ :0 Axel BRISSE
:liquid.oftc.net 315 axbrisse1 #lololol :End of /WHO list.

352 WHOREPLY
315 ENDOFWHO
402 NOSUCHSERVER

TODO correct ip
TODO @ before op
TODO WHO nickname
TODO WHO mask
*/

static std::string getChannelPrefix(int mode)
{
	if (mode == OPERATOR)
		return "@";
	if (mode == PROTECTED)
		return "&";
	if (mode == FOUNDER)
		return "~";
	return "";
}

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
			std::vector<Client*> users = channel->getChannelUsers();
			for (std::vector<Client*>::iterator it = users.begin(); it != users.end(); it++)
			{
				Client* user = *it;
				std::string prefix = getChannelPrefix(channel->getChannelUserMode(user));
				client->reply(RPL_WHOREPLY,
							  name + " " + user->getUsername() + " " + user->getIp() + " " +
								  SERVERHOSTNAME + " " + user->getNickname() + " H" + prefix,
							  user->getRealname());
			}
		}
	}
	else
	{
	}
	client->reply(RPL_ENDOFWHO, name);
}
