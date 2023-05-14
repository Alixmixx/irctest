#include "Server.hpp"

void Server::handleCap(Client *client, std::vector<std::string> arguments)
{
	(void)client;
	if (arguments.size() == 0)
	{
		replyMessage(client, "ERR_NEEDMOREPARAMS", "CAP");
		return;
	}
	else if (arguments[0] == "LS")
	{
		if (DEBUG)
			std::cout << "LS ça marche" << std::endl;
		return;
	}
	else
	{
		// ERR_INVALIDCAPCMD
	}
}