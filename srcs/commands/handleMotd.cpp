#include "Server.hpp"

void Server::handleMotd(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() >= 1 && arguments[0] != SERVERHOSTNAME)
		return client->reply(ERR_NOSUCHSERVER, arguments[0]);
	client->reply(RPL_MOTDSTART, SERVERHOSTNAME);
	std::ifstream motdFile("motd.txt");
	if (motdFile.is_open())
	{
		std::string line;
		while (getline(motdFile, line))
			client->reply(RPL_MOTD, line);
		motdFile.close();
	}
	client->reply(RPL_ENDOFMOTD);
}