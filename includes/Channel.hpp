#pragma once

#include "ft_irc.hpp"

class Client;
class Server;

class Channel
{
public:
	Channel(Server *server, std::string &name);
	~Channel();

	const std::string &getName() const;
	const std::string &getTopic() const;
	const std::string &getPassword() const;
	const std::string &getMode() const;
	const std::string &getKey() const;
	std::vector<Client *> &getChannelUsers();
	bool isPasswordProtected() const;
	bool isOperator(Client *client) const;
	bool isBanned(Client *client) const;
	bool isInvited(Client *client) const;
	bool isOnChannel(Client *client) const;
	bool isInviteOnly() const;

	void setTopic(const std::string &topic);
	void setPassword(const std::string &password);
	void setMode(const std::string &mode);
	void setKey(const std::string &key);

	void removeClientFromChannel(Client *client);

	void addChannelUser(Client *client);
	void removeChannelUser(Client *client);

	void addChannelOperator(Client *client);
	void removeChannelOperator(Client *client);

	void addChannelBan(Client *client);
	void removeChannelBan(Client *client);

	void addChannelInvite(Client *client);
	void removeChannelInvite(Client *client);

private:
	std::string _name;
	std::string _topic;
	std::string _password;
	std::string _mode;
	std::string _key;

	bool _inviteOnly;

	Server *_server;
	std::vector<Client *> _channelOperators;
	std::vector<Client *> _channelUsers;
	std::vector<Client *> _channelBans;
	std::vector<Client *> _channelInvites;
};
