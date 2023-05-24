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

	std::vector<Client*>&	getChannelUsers();
	std::map<Client*, int>& getChannelUsersModes();
	Client*					getHighestGradedUser();

	int			getChannelUserMode(Client* client) const;
	std::string getChannelPrefix(Client* client) const;

	bool isOnChannel(Client* client) const;

	bool isInviteOnly() const;
	bool isPasswordProtected() const;
	bool isUserLimitSet() const;
	bool isSecret() const;
	bool isTopicProtected() const;

	void setMode(int mode, bool sign);

	void setTopic(Client* client, const std::string& topic);
	void setPassword(const std::string& password);

	void setClientMode(Client* client, int mode);

	void removeClientFromChannel(Client* client);

	void addChannelUser(Client* client, int mode = USER);

	void addChannelOperator(Client* client);
	void removeChannelOperator(Client* client);

	void addChannelBan(Client* client);
	void removeChannelBan(Client* client);

	void addChannelInvite(Client* client);
	void removeChannelInvite(Client* client);

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
};
