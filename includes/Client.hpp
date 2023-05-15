#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <string>
#include <netinet/in.h>

class Server;

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

		// Getters
		int			getSocket() const;
		bool		getIsRegistered() const;
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealname() const;
		std::string getHostname() const;
		std::string getMessage() const;
		Server		*getServer() const;

	private:
		int				_clientSocket;
		bool			_isRegistered;

		struct sockaddr_in		_clientAddress;
		
		std::string 	_nickname;
		std::string 	_username;
		std::string 	_realname;
		std::string 	_hostname;

		std::string 	_message;

		//std::vector<Channel *> _channels;

		Server *_server;
};

#endif