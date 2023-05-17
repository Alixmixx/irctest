#pragma once

#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"

class Server;

class Channel;

class Client
{
	public:
		Client(Server *server, int socketFd, sockaddr_in clientAddress);
		~Client();

		// Setters
		void setNickname(std::string nickname);
		void setIsRegistered(bool isRegistered);
		void setUsername(std::string username);
		void setRealname(std::string realname);
		void setHostname(std::string hostname);
		void setMessage(std::string message);
		void setChannelCount(int channelCount);
		void addChannel(Channel *channel);

		// Getters
		int			getSocket() const;
		int			getChannelCount() const;
		bool		getIsRegistered() const;
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealname() const;
		std::string getHostname() const;
		std::string getMessage() const;
		Server		*getServer() const;

		// Reply
		void reply(std::string replyCode);
		void reply(std::string replyCode, std::string arg1);
		void reply(std::string replyCode, std::string arg1, std::string arg2);
		void reply(std::string replyCode, std::string arg1, std::string arg2, std::string arg3);

	private:
		int				_clientSocket;
		int				_channelCount;
		bool			_isRegistered;

		struct sockaddr_in		_clientAddress;

		std::string 	_nickname;
		std::string 	_username;
		std::string 	_realname;
		std::string 	_hostname;

		std::string 	_message;

		std::vector<Channel *> _channels;
		Server *_server;
};
