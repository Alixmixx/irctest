#include "Server.hpp"

// TODO full address instead of just IP address
// /who erik example on octf:
// * ~erik h-158-174-164-251.A357.priv.bahnhof.se kinetic.oftc.net erik H :3 Erik Meijer

static std::string getChannelPrefix(int mode)
{
	if (mode == OPERATOR)
		return "@";
/* 	if (mode == PROTECTED)
		return "&"; */
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
				Client*		user = *it;
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
		Client* target = getClient(name);
		if (target != NULL)
			client->reply(RPL_WHOREPLY,
						  "* " + target->getUsername() + " " + target->getIp() + " " +
							  SERVERHOSTNAME + " " + target->getNickname() + " H",
						  target->getRealname());
	}
	client->reply(RPL_ENDOFWHO, name);
}
