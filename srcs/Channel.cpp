#include "Channel.hpp"

Channel::Channel(Server *server, std::string &name)
	: _name(name),
	  _topic("Change topic with /topic <new topic>"),
	  _password(""),
	  _mode(""),
	  _key(""),
	  _inviteOnly(false),
	  _isSecret(false),
	  _server(server)
{
	(void)_server;
}

Channel::~Channel()
{
	_channelUsers.clear();
	_channelUsersModes.clear();
}

// Getters

const std::string &Channel::getName() const { return _name; }

const std::string &Channel::getTopic() const { return _topic; }

const std::string &Channel::getPassword() const { return _password; }

const std::string &Channel::getKey() const { return _key; }

const std::string &Channel::getTopicSetter() const { return _topicSetter; }

time_t Channel::getTopicTimestamp() const { return _topicTimestamp; }

std::vector<Client *> &Channel::getChannelUsers() { return _channelUsers; }

std::map<Client *, int> &Channel::getChannelUsersModes() { return _channelUsersModes; }

int Channel::getChannelUserMode(Client *client) const
{
	std::map<Client *, int>::const_iterator it = _channelUsersModes.find(client);
	if (it != _channelUsersModes.end())
		return (it->second);
	return (NOTINCHANNEL);
}

bool Channel::isOnChannel(Client *client) const
{
	std::vector<Client *>::const_iterator it = _channelUsers.begin();
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

void Channel::setTopic(Client *client, const std::string &topic)
{
	_topic = topic;
	_topicSetter = client->getNickname();
	_topicTimestamp = std::time(NULL);
}

void Channel::setPassword(const std::string &password) { _password = password; }

void Channel::setKey(const std::string &key) { _key = key; }

void Channel::setClientMode(Client *client, int mode) { _channelUsersModes[client] = mode; }

// Methods

void Channel::addChannelUser(Client *client) {
	_channelUsers.push_back(client);
	_channelUsersModes[client] = USER;
	client->addChannel(this);
}

void Channel::addChannelUser(Client *client, int mode) {
	_channelUsers.push_back(client);
	_channelUsersModes[client] = mode;
	client->addChannel(this);
}

void Channel::removeClientFromChannel(Client *client)
{
	removeChannelUser(client);
}

void Channel::removeChannelUser(Client *client)
{
	std::vector<Client *>::iterator it = _channelUsers.begin();
	while (it != _channelUsers.end())
	{
		if (*it == client)
		{
			_channelUsers.erase(it);
			break;
		}
		it++;
	}

	std::map<Client *, int>::iterator it2 = _channelUsersModes.find(client);
	if (it2 != _channelUsersModes.end())
		_channelUsersModes.erase(it2);

	client->leaveChannel(this);
}

