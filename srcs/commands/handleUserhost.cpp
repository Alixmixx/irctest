#include "Server.hpp"

void Server::handleUserhost(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
		return client->reply(ERR_NEEDMOREPARAMS, "USERHOST");
	std::vector<std::string> userhosts;
	for (size_t i = 0; i < 5; ++i)
	{
		if (i >= arguments.size())
			break;
		std::string nickname = arguments[i];
		Client* target = getClient(nickname);
		if (target != NULL)
			userhosts.push_back(target->getNickname() + "=+" + target->getHostname());
	}
	client->reply(RPL_USERHOST, strjoin(userhosts, ' '));
}
