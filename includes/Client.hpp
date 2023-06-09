#pragma once

#include "ft_irc.hpp"

class Client {
public:
	Client(Server* server, int socketFd, sockaddr_in clientAddress);
	~Client();

	void setNickname(std::string nickname);
	void setIsRegistered(bool isRegistered);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setHostname(std::string hostname);
	void setIsInvisible(bool invisible);
	void setLastAction();
	void setPasswordCorrect();
	void setAdmin(bool isAdmin);

	void addChannel(Channel* channel);
	void leaveChannel(Channel* channel);

	int					  getSocket() const;
	bool				  isRegistered() const;
	bool				  isInvisible() const;
	bool				  isPasswordCorrect() const;
	bool				  isAdmin() const;
	std::string			  getNickname() const;
	std::string			  getUsername() const;
	std::string			  getRealname() const;
	std::string			  getHostname() const;
	std::string			  getPrefix() const;
	std::string			  getIp() const;
	std::string			  getModeString() const;
	std::vector<Channel*> getChannels() const;
	time_t				  getSignonTime() const;
	time_t				  getLastAction() const;
	sockaddr_in			  getClientAddress() const;
	Server*				  getServer() const;

	void reply(std::string replyMessage) const;
	void reply(std::string replyMessage, ReplyCode replyCode) const;
	void reply(ReplyCode replyCode, std::string arg1 = "", std::string arg2 = "",
			   std::string arg3 = "", std::string arg4 = "") const;

	std::string _message;

private:
	int _clientSocket;
	int _modes;

	bool _isRegistered;
	bool _isInvisible;
	bool _isPasswordCorrect;
	bool _isAdmin;

	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _hostname;
	std::string _prefix;

	sockaddr_in _clientAddress;

	time_t _signonTime;
	time_t _lastAction;

	std::vector<Channel*> _channels;
	Server*				  _server;
};

std::ostream& operator<<(std::ostream& os, const Client& client);