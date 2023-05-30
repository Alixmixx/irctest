#include "Server.hpp"

void Server::handleWho(Client *client, std::vector<std::string> arguments)
{
	if (arguments.empty())
		return client->reply(ERR_NEEDMOREPARAMS, "WHO");
	std::string name = arguments[0];
	if (name[0] == '#' || name[0] == '&')
	{
		Channel *channel = getChannel(name);
		if (channel != NULL && channel->isOnChannel(client))
		{
			std::vector<Client *> users = channel->getChannelUsers();
			for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
			{
				Client *user = *it;
				std::string prefix = channel->getChannelPrefix(user);
				client->reply(RPL_WHOREPLY,
							  name + " " + user->getUsername() + " " + user->getHostname() + " " +
								  SERVERHOSTNAME + " " + user->getNickname() + " H" + prefix,
							  user->getRealname());
			}
		}
	}
	else
	{
		Client *target = getClient(name);
		if (target != NULL)
			client->reply(RPL_WHOREPLY,
						  "* " + target->getUsername() + " " + target->getHostname() + " " +
							  SERVERHOSTNAME + " " + target->getNickname() + " H",
						  target->getRealname());
	}
	client->reply(RPL_ENDOFWHO, name);
}
