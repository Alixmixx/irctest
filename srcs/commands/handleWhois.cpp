#include "Server.hpp"

static std::vector<std::string> getChannelsToReply(Client* client, Client* target)
{
	std::vector<Channel*>	 channels = target->getChannels();
	std::vector<std::string> channelsToReply;
	for (std::vector<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel* channel = *it;
		if (!channel->isSecret() || channel->getChannelUserMode(client) >= INVITED)
			channelsToReply.push_back(channel->getChannelPrefix(target) + channel->getName());
	}
	return channelsToReply;
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
		client->reply(ERR_NOSUCHNICK, nickname);
		client->reply(RPL_ENDOFWHOIS, nickname);
		return;
	}
	client->reply(RPL_WHOISUSER, target->getNickname(), target->getUsername(),
				  target->getHostname(), target->getRealname());
	std::vector<std::string> channelsToReply = getChannelsToReply(client, target);
	if (!channelsToReply.empty())
		client->reply(RPL_WHOISCHANNELS, target->getNickname(), strjoin(channelsToReply, ' '));
	client->reply(RPL_WHOISSERVER, target->getNickname(), SERVERHOSTNAME, SERVERLOC2);
	client->reply(RPL_WHOISACTUALLY, target->getNickname(), target->getHostname());
	client->reply(RPL_WHOISIDLE, target->getNickname(),
				  toString(std::time(NULL) - target->getLastAction()),
				  toString(target->getSignonTime()));
	client->reply(RPL_ENDOFWHOIS, target->getNickname());
}
