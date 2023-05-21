#include "Server.hpp"

void Server::handleOper(Client* client, std::vector<std::string> arguments)
{
	(void)arguments;
	client->reply(ERR_NOOPERHOST, client->getNickname());
}