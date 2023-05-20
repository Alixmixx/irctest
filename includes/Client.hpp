#pragma once

#include "ft_irc.hpp"

class Channel;
class Server;

class Client {
public:
	Client(Server* server, int socketFd, sockaddr_in clientAddress);
	~Client();

	void setNickname(std::string nickname);
	void setIsRegistered(bool isRegistered);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setHostname(std::string hostname);
	void setMessage(std::string message);
	void setIsInvisible(bool invisible);
	void addChannel(Channel* channel);
	void leaveChannel(Channel* channel);

	int							getSocket() const;
	bool						IsRegistered() const;
	bool						IsInvisible() const;
	std::string 				getNickname() const;
	std::string 				getUsername() const;
	std::string 				getRealname() const;
	std::string 				getHostname() const;
	std::string 				getPrefix() const;
	std::string 				getMessage() const;
	Server*						getServer() const;
	const std::vector<Channel*>	&getChannels() const;

	void reply(std::string replyMessage) const;
	void reply(std::string replyMessage, ReplyCode replyCode) const;
	void reply(ReplyCode replyCode, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "") const;

private:
	int	 _clientSocket;
	bool _isRegistered;
	bool _isInvisible;

	struct sockaddr_in _clientAddress;

	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _hostname;
	std::string _prefix;

	std::string _message;

	std::vector<Channel*> _channels;
	Server*				  _server;
};

std::ostream& operator<<(std::ostream& os, const Client& client);