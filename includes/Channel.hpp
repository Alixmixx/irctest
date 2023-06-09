#pragma once

#include "ft_irc.hpp"

class Channel {
public:
	Channel(Server* server, std::string& name, time_t creationTime);
	~Channel();

	const std::string& getName() const;
	const std::string& getTopic() const;
	const std::string& getPassword() const;
	time_t			   getCreationTime() const;
	const std::string& getTopicSetter() const;
	std::string		   getModeString() const;
	time_t			   getTopicTimestamp() const;
	int				   getUserLimit() const;

	std::vector<Client*>&	getChannelUsers();
	std::map<Client*, int>& getChannelUsersModes();
	Client*					getHighestGradedUser(Client* client) const;

	std::vector<std::string>& getBanList();

	int			getChannelUserMode(Client* client) const;
	std::string getChannelPrefix(Client* client) const;

	bool isOnChannel(Client* client) const;

	bool isInviteOnly() const;
	bool isPasswordProtected() const;
	bool isUserLimitSet() const;
	bool isSecret() const;
	bool isTopicProtected() const;
	bool isModerated() const;
	bool isUserBanned(Client* client) const;

	void setMode(int mode, bool sign);
	void setUserLimit(int limit);

	void setTopic(Client* client, const std::string& topic);
	void setPassword(const std::string& password);

	void setClientMode(Client* client, int mode);

	void removeClientFromChannel(Client* client);

	void addChannelUser(Client* client, int mode = USER);

private:
	std::string _name;
	std::string _topic;
	std::string _password;
	time_t		_creationTime;

	std::string _topicSetter;
	time_t		_topicTimestamp;
	int			_userLimit;

	int _modes;

	Server*				   _server;
	std::map<Client*, int> _channelUsersModes;
	std::vector<Client*>   _channelUsers;
	std::vector<std::string> _banList;
};
