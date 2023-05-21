#include "Server.hpp"

void Server::handleMotd(Client* client, std::vector<std::string> arguments)
{
	if (arguments.size() > 0 && toLowerCase(arguments[0]) != toLowerCase(_serverName))
		return client->reply(ERR_NOSUCHSERVER, arguments[0]);

	if (_serverMotd == "")
		return client->reply(ERR_NOMOTD);

	client->reply(RPL_MOTDSTART, _serverName);

	std::vector<std::string> lines = split(_serverMotd, '\n');
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if ((*it).size() > 80)
		{
			std::string line = *it;
			while (line.size() > 80)
			{
				client->reply(RPL_MOTD, line.substr(0, 80));
				line = line.substr(80);
			}
			client->reply(RPL_MOTD, line);
		}
		else
			client->reply(RPL_MOTD, *it);
	}

	client->reply(RPL_ENDOFMOTD);
}