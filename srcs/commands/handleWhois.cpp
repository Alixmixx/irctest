#include "Server.hpp"

// TODO add @ before channels where target is operator
// TODO handle other prefixes than #
static std::string getChannelsString(Client* target)
{
	std::vector<Channel*> channels = target->getChannels();
	std::string s;
	for (std::vector<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
		s += (*it)->getName() + " ";
	if (!s.empty())
		s.pop_back();
	return s;
}

void Server::handleWhois(Client* client, std::vector<std::string> arguments)
{
	if (arguments.empty())
		return client->reply(ERR_NONICKNAMEGIVEN);
	if (arguments.size() >= 2 && arguments[0] != SERVERHOSTNAME)
		return client->reply(ERR_NOSUCHSERVER, arguments[0]);

	std::string nickname = arguments[arguments.size() >= 2];
	Client*		target = getClient(nickname);
	if (target == NULL)
	{
		client->reply(ERR_NOSUCHNICK(nickname));
		client->reply(RPL_ENDOFWHOIS(nickname));
		return;
	}
	client->reply(RPL_WHOISUSER, target->getNickname(), target->getUsername(),
				  target->getHostname(), target->getRealname());
	client->reply(RPL_WHOISCHANNELS, target->getNickname(), getChannelsString(target));
	client->reply(RPL_WHOISSERVER, target->getNickname(), SERVERHOSTNAME, SERVERLOC2);
	client->reply(RPL_WHOISACTUALLY, target->getNickname(), "127.0.0.1"); // TODO
	client->reply(RPL_WHOISIDLE, target->getNickname(), "42", "1684708203"); // TODO
	client->reply(RPL_ENDOFWHOIS, target->getNickname());
}
