#include "Channel.hpp"

Channel::Channel(Server *server, std::string &name, time_t creationTime)
	: _name(name), _topic(""), _password(""), _creationTime(creationTime), _topicSetter(""),
	  _topicTimestamp(std::time(NULL)), _userLimit(MAX_USERS_PER_CHANNEL), _modes(0),
	  _server(server)
{
}

Channel::~Channel()
{
	_channelUsers.clear();
	_channelUsersModes.clear();
}

// Getters

const std::string &Channel::getName() const { return _name; }

time_t Channel::getCreationTime() const { return _creationTime; }

const std::string &Channel::getTopic() const { return _topic; }

const std::string &Channel::getPassword() const { return _password; }

const std::string &Channel::getTopicSetter() const { return _topicSetter; }

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
	return (mode);
}

time_t Channel::getTopicTimestamp() const { return _topicTimestamp; }

int Channel::getUserLimit() const { return _userLimit; }

std::vector<Client *> &Channel::getChannelUsers() { return _channelUsers; }

std::map<Client *, int> &Channel::getChannelUsersModes() { return _channelUsersModes; }

int Channel::getChannelUserMode(Client *client) const
{
	std::map<Client *, int>::const_iterator it = _channelUsersModes.find(client);
	if (it != _channelUsersModes.end())
		return (it->second);
	return (NOTINCHANNEL);
}

std::vector<std::string> &Channel::getBanList() { return _banList; }

std::string Channel::getChannelPrefix(Client *client) const
{
	int mode = getChannelUserMode(client);
	return mode == FOUNDER ? "~" : mode == OPERATOR ? "@" : "";
}

bool Channel::isOnChannel(Client *client) const
{
	std::vector<Client *>::const_iterator it = _channelUsers.begin();
	while (it != _channelUsers.end())
	{
		if (*it == client)
			return true;
		it++;
	}
	return false;
}

bool Channel::isInviteOnly() const { return (_modes & M_INVITE); }

bool Channel::isPasswordProtected() const { return (_modes & M_KEY); }

bool Channel::isUserLimitSet() const { return (_modes & M_LIMITED); }

bool Channel::isSecret() const { return (_modes & M_SECRET); }

bool Channel::isTopicProtected() const { return (_modes & M_PROTECTED); }

bool Channel::isModerated() const { return (_modes & M_MODERATED); }

static bool wildcardMatch(const std::string &pattern, const std::string &text)
{
	if (pattern.empty() && text.empty())
		return true;

	if (pattern.empty())
		return false;

	if (pattern[0] == '*')
		return wildcardMatch(pattern.substr(1), text) || wildcardMatch(pattern, text.substr(1));

	if (!text.empty() && pattern[0] == text[0])
		return wildcardMatch(pattern.substr(1), text.substr(1));

	return false;
}

bool Channel::isUserBanned(Client *client) const
{
	if (getChannelUserMode(client) >= OPERATOR)
		return false;

	for (std::vector<std::string>::const_iterator it = _banList.begin(); it != _banList.end(); it++)
	{
		std::string nickname = it->substr(0, it->find("!"));
		std::string username = it->substr(it->find("!") + 1, it->find("@") - it->find("!") - 1);
		std::string hostname = it->substr(it->find("@") + 1);

		if (wildcardMatch(nickname, client->getNickname()) && wildcardMatch(username, client->getUsername()) && wildcardMatch(hostname, client->getHostname()))
			return true;
	}
	return false;
}

// Setters

void Channel::setTopic(Client *client, const std::string &topic)
{
	_topic = topic;
	_topicSetter = client->getPrefix().substr(1);
	_topicTimestamp = std::time(NULL);
}

void Channel::setPassword(const std::string &password) { _password = password; }

void Channel::setClientMode(Client *client, int mode) { _channelUsersModes[client] = mode; }

void Channel::setMode(int mode, bool sign)
{
	if (sign == PLUS)
		_modes |= mode;
	else if (sign == MINUS)
		_modes &= ~mode;
}

void Channel::setUserLimit(int limit) { _userLimit = limit; }

// Methods

void Channel::addChannelUser(Client *client, int mode)
{
	_channelUsers.push_back(client);
	_channelUsersModes[client] = mode;
	client->addChannel(this);
}

Client *Channel::getHighestGradedUser(Client *client) const
{
	int grade = INVITED;
	Client *highestGradedUser = NULL;

	for (std::map<Client *, int>::const_iterator it = _channelUsersModes.begin(); it != _channelUsersModes.end(); it++)
	{
		if (it->first == client)
			continue;
		if (it->second > grade)
		{
			highestGradedUser = it->first;
			grade = it->second;
		}
	}
	if (grade < OPERATOR) // if there is no operator, return the first user
		return (highestGradedUser);
	return (NULL); // if there is at least one operator, return NULL
}

void Channel::removeClientFromChannel(Client *client)
{
	for (std::map<Client *, int>::iterator it = _channelUsersModes.begin(); it != _channelUsersModes.end(); it++)
	{
		/* 		if (it->first == client)
				{
					it->second = NOTINCHANNEL;
					break;
				} */
		if (it->first == client)
		{
			if (it->second >= OPERATOR && getChannelUsers().size() > 1 && getHighestGradedUser(client) != NULL)
			{
				std::vector<std::string> params;
				params.push_back(_name);
				params.push_back("+o");
				params.push_back(getHighestGradedUser(client)->getNickname());
				_server->handleMode(it->first, params);
			}
			_channelUsersModes.erase(it);
			break;
		}
	}

	for (std::vector<Client *>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (*it == client)
		{
			_channelUsers.erase(it);
			break;
		}
	}

	client->leaveChannel(this);

	if (_channelUsers.empty())
		_server->removeChannel(this);
}
