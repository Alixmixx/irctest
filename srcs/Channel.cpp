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
	_channelOperators.clear();
}

// Getters

const std::string &Channel::getName() const { return _name; }

const std::string &Channel::getTopic() const { return _topic; }

const std::string &Channel::getPassword() const { return _password; }

const std::string &Channel::getKey() const { return _key; }

const std::string &Channel::getTopicSetter() const { return _topicSetter; }

const std::string &Channel::getTopicTimestamp() const { return _topicTimestamp; }

std::vector<Client *> &Channel::getChannelUsers() { return _channelUsers; }

bool Channel::isOperator(Client *client) const
{
	std::vector<Client *>::const_iterator it = _channelOperators.begin();
	while (it != _channelOperators.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::isBanned(Client *client) const
{
	std::vector<Client *>::const_iterator it = _channelBans.begin();
	while (it != _channelBans.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::isInvited(Client *client) const
{
	std::vector<Client *>::const_iterator it = _channelInvites.begin();
	while (it != _channelInvites.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
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
	_topicTimestamp = getCurrentDateTime();
}

void Channel::setPassword(const std::string &password) { _password = password; }

void Channel::setKey(const std::string &key) { _key = key; }

// Methods

void Channel::addChannelUser(Client *client) {
	_channelUsers.push_back(client);
	client->addChannel(this);
}

void Channel::removeClientFromChannel(Client *client)
{
	removeChannelUser(client);
	removeChannelOperator(client);
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
}

void Channel::addChannelOperator(Client *client) { _channelOperators.push_back(client); }

void Channel::removeChannelOperator(Client *client)
{
	std::vector<Client *>::iterator it = _channelOperators.begin();
	while (it != _channelOperators.end())
	{
		if (*it == client)
		{
			_channelOperators.erase(it);
			break;
		}
		it++;
	}
}

void Channel::addChannelBan(Client *client) { _channelBans.push_back(client); }

void Channel::removeChannelBan(Client *client)
{
	std::vector<Client *>::iterator it = _channelBans.begin();
	while (it != _channelBans.end())
	{
		if (*it == client)
		{
			_channelBans.erase(it);
			break;
		}
		it++;
	}
}

void Channel::addChannelInvite(Client *client) { _channelInvites.push_back(client); }

void Channel::removeChannelInvite(Client *client)
{
	std::vector<Client *>::iterator it = _channelInvites.begin();
	while (it != _channelInvites.end())
	{
		if (*it == client)
		{
			_channelInvites.erase(it);
			break;
		}
		it++;
	}
}
