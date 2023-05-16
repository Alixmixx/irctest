#include "Channel.hpp"

Channel::Channel(const std::string &name) : _name(name) {}

Channel::~Channel() {}

// Getters

const std::string &Channel::getName() const { return _name; }

const std::string &Channel::getTopic() const { return _topic; }

const std::string &Channel::getPassword() const { return _password; }

const std::string &Channel::getMode() const { return _mode; }

const std::string &Channel::getKey() const { return _key; }

std::vector<Client *> &Channel::getChannelUsers() { return _channelUsers; }

// Setters

void Channel::setTopic(const std::string &topic) { _topic = topic; }

void Channel::setPassword(const std::string &password) { _password = password; }

void Channel::setMode(const std::string &mode) { _mode = mode; }

void Channel::setKey(const std::string &key) { _key = key; }

// Methods

void Channel::addChannelUser(Client *client) { _channelUsers.push_back(client); }

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
