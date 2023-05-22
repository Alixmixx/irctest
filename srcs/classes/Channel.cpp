#include "Channel.hpp"

Channel::Channel(Server* server, std::string& name)
	: _name(name),
	  _topic(""),
	  _password(""),
	  _mode(""),
	  _key(""),
	  _topicSetter(""),
	  _topicTimestamp(std::time(NULL)),
	  _isTopicProtected(false),
	  _inviteOnly(false),
	  _isSecret(false),
	  _server(server)
{
}

Channel::~Channel()
{
	_channelUsers.clear();
	_channelUsersModes.clear();
}

// Getters

const std::string& Channel::getName() const { return _name; }

const std::string& Channel::getTopic() const { return _topic; }

const std::string& Channel::getPassword() const { return _password; }

const std::string& Channel::getKey() const { return _key; }

const std::string& Channel::getTopicSetter() const { return _topicSetter; }

time_t Channel::getTopicTimestamp() const { return _topicTimestamp; }

bool Channel::isTopicProtected() const { return (_isTopicProtected); }

std::vector<Client*>& Channel::getChannelUsers() { return _channelUsers; }

std::map<Client*, int>& Channel::getChannelUsersModes() { return _channelUsersModes; }

int Channel::getChannelUserMode(Client* client) const
{
	std::map<Client*, int>::const_iterator it = _channelUsersModes.find(client);
	if (it != _channelUsersModes.end())
		return (it->second);
	return (NOTINCHANNEL);
}

bool Channel::isOnChannel(Client* client) const
{
	std::vector<Client*>::const_iterator it = _channelUsers.begin();
	while (it != _channelUsers.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::isPasswordProtected() const { return (_password != ""); }

bool Channel::isSecret() const { return (_isSecret); }

bool Channel::isInviteOnly() const { return (_inviteOnly); }

// Setters

void Channel::setTopic(Client* client, const std::string& topic)
{
	_topic = topic;
	_topicSetter = client->getNickname();
	_topicTimestamp = std::time(NULL);
}

void Channel::setPassword(const std::string& password) { _password = password; }

void Channel::setKey(const std::string& key) { _key = key; }

void Channel::setClientMode(Client* client, int mode) { _channelUsersModes[client] = mode; }

void Channel::setIsTopicProtected(bool isTopicProtected) { _isTopicProtected = isTopicProtected; }

// Methods

void Channel::addChannelUser(Client* client)
{
	_channelUsers.push_back(client);
	_channelUsersModes[client] = USER;
	client->addChannel(this);
}

void Channel::addChannelUser(Client* client, int mode)
{
	_channelUsers.push_back(client);
	_channelUsersModes[client] = mode;
	client->addChannel(this);
}

Client* Channel::getHighestGradedUser()
{
	int		grade = INVITED;
	Client* client = NULL;

	for (std::map<Client*, int>::iterator it = _channelUsersModes.begin(); it != _channelUsersModes.end(); it++)
	{
		if (it->second > grade && it->second != FOUNDER)
		{
			client = it->first;
			grade = it->second;
		}
	}
	return (client);
}

void Channel::removeClientFromChannel(Client* client)
{

	for (std::vector<Client*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (*it == client)
		{
			_channelUsers.erase(it);
			break;
		}
	}

	for (std::map<Client*, int>::iterator it = _channelUsersModes.begin(); it != _channelUsersModes.end(); it++)
	{
		if (it->first == client)
		{
			if (it->second == FOUNDER && getChannelUsers().size() > 1)
				setClientMode(getHighestGradedUser(), FOUNDER);
			_channelUsersModes.erase(it);
			break;
		}
	}

	client->leaveChannel(this);

	if (getChannelUsers().size() == 0)
		_server->removeChannel(this);
}
