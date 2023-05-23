#include "Channel.hpp"

Channel::Channel(Server* server, std::string& name, time_t creationTime)
	: _name(name),
	  _topic(""),
	  _password(""),
	  _creationTime(creationTime),
	  _topicSetter(""),
	  _topicTimestamp(std::time(NULL)),
	  _userLimit(MAX_USERS_PER_CHANNEL),
	  _modes(0),
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

time_t Channel::getCreationTime() const { return _creationTime; }

const std::string& Channel::getTopic() const { return _topic; }

const std::string& Channel::getPassword() const { return _password; }

const std::string& Channel::getTopicSetter() const { return _topicSetter; }

std::string Channel::getModeString() const
{
	std::string mode = "+";
	if (isInviteOnly())
		mode += "i";
	if (isPasswordProtected())
		mode += "k";
	if (isUserLimitSet())
		mode += "l";
	if (isSecret())
		mode += "s";
	if (isTopicProtected())
		mode += "t";
	if (mode == "+")
		mode = "";  // TODO verifier si c'est bien ca
	return (mode);
}

time_t Channel::getTopicTimestamp() const { return _topicTimestamp; }

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

bool Channel::isInviteOnly() const { return (_modes & M_INVITE); }

bool Channel::isPasswordProtected() const { return (_modes & M_KEY); }

bool Channel::isUserLimitSet() const { return (_modes & M_LIMITED); }

bool Channel::isSecret() const { return (_modes & M_SECRET); }

bool Channel::isTopicProtected() const { return (_modes & M_PROTECTED); }

// Setters

void Channel::setTopic(Client* client, const std::string& topic)
{
	_topic = topic;
	_topicSetter = client->getPrefix().substr(1);
	_topicTimestamp = std::time(NULL);
}

void Channel::setPassword(const std::string& password) { _password = password; }

void Channel::setClientMode(Client* client, int mode) { _channelUsersModes[client] = mode; }

void Channel::setMode(int mode, bool sign)
{
	if (sign == PLUS)
		_modes |= mode;
	else if (sign == MINUS)
		_modes &= ~mode;

	if (mode & M_LIMITED)
		_userLimit = _channelUsers.size();
}

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
