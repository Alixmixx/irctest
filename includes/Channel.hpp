#pragma once

#include "ft_irc.hpp"

class Client;
class Server;

class Channel {
public:
	Channel(Server* server, std::string& name);
	~Channel();

	const std::string& getName() const;
	const std::string& getTopic() const;
	const std::string& getPassword() const;
	const std::string& getMode() const;
	const std::string& getKey() const;
	const std::string& getTopicSetter() const;
	time_t			   getTopicTimestamp() const;

	std::vector<Client*>&	getChannelUsers();
	std::map<Client*, int>& getChannelUsersModes();

	int getChannelUserMode(Client* client) const;

	bool isOnChannel(Client* client) const;
	bool isPasswordProtected() const;
	bool isSecret() const;
	bool isInviteOnly() const;

	void setTopic(Client* client, const std::string& topic);
	void setPassword(const std::string& password);
	void setMode(const std::string& mode);
	void setKey(const std::string& key);

	void setClientMode(Client* client, int mode);

	void removeClientFromChannel(Client* client);

	void addChannelUser(Client* client);
	void addChannelUser(Client* client, int mode);
	void removeChannelUser(Client* client);

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
	std::string _mode;
	std::string _key;

	std::string _topicSetter;
	time_t		_topicTimestamp;
	bool		_inviteOnly;
	bool		_isSecret;

	Server*				   _server;
	std::map<Client*, int> _channelUsersModes;
	std::vector<Client*>   _channelUsers;
};
