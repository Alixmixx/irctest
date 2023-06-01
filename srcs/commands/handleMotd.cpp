#include "Server.hpp"

void Server::handleMotd(Client *client, std::vector<std::string> arguments)
{

	if (arguments.size() >= 1 && toLowerCase(arguments[0]) != toLowerCase(SERVERHOSTNAME) && toLowerCase(arguments[0]) != toLowerCase(SERVERNAME))
		return client->reply(ERR_NOSUCHSERVER, arguments[0]);

	if (isFileModified("motd.txt", _motdCreationTime))
	{
		std::ifstream motdFile("motd.txt");
		if (motdFile.is_open())
		{
			std::stringstream ss;
			char c;
			while (motdFile.get(c))
					ss << c;  // watch out for unicode
			_motd = ss.str();
			motdFile.close();
		}
		else
			return client->reply(ERR_NOMOTD);
	}

	if (_motd.empty())
		return client->reply(ERR_NOMOTD);
	client->reply(RPL_MOTDSTART, SERVERHOSTNAME);
	std::vector<std::string> motd = split(_motd, '\n');
	for (std::vector<std::string>::iterator it = motd.begin(); it != motd.end(); ++it)
		client->reply(RPL_MOTD, *it);
	client->reply(RPL_ENDOFMOTD);
}