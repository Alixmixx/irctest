#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>

class Channel
{
public:
	Channel(const std::string &name);
	~Channel();

	const std::string &getName() const;
	const std::string &getTopic() const;
	const std::string &getPassword() const;
	const std::string &getMode() const;
	const std::string &getKey() const;
	std::vector<Client *> &getChannelUsers();

	void setTopic(const std::string &topic);
	void setPassword(const std::string &password);
	void setMode(const std::string &mode);
	void setKey(const std::string &key);

	void addChannelUser(Client *client);
	void removeChannelUser(Client *client);

private:
	const std::string _name;
	std::string _topic;
	std::string _password;
	std::string _mode;
	std::string _key;

	Client *_channelCreator;
	std::vector<Client *> _channelUsers;
};

#endif