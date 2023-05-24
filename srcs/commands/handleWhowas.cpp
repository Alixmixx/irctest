#include "Server.hpp"

void Server::handleWhowas(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
		return client->reply(ERR_NONICKNAMEGIVEN);
	std::string nickname = arguments[0];
	int count = arguments.size() >= 2 ? stoi(arguments[1]) : 0;
	bool wasNoSuchNick = true;
	for (std::vector<FormerClient*>::reverse_iterator it = _formerClients.rbegin();
		it != _formerClients.rend(); ++it)
	{
		FormerClient* target = *it;
		if (target->nickname == nickname)
		{
			client->reply(RPL_WHOWASUSER, target->nickname, target->username, target->hostname, target->realname);
			client->reply(RPL_WHOISSERVER, target->nickname, SERVERHOSTNAME, SERVERLOC2);
			wasNoSuchNick = false;
			if (count == 1)
				break;
			else if (count > 1)
				--count;
		}
	}
	if (wasNoSuchNick)
		client->reply(ERR_WASNOSUCHNICK, nickname);
	client->reply(RPL_ENDOFWHOWAS, nickname);
}
