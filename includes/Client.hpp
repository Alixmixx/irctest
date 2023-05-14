#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <string>

class Server;

class Client
{
	public:
		Client(Server *server, int socketFd, std::string hostname);
		~Client();

		// Setters
		void setNickname(std::string nickname);
		void setIsRegistered(bool isRegistered);
		void setUsername(std::string username);
		void setRealname(std::string realname);
		void setHostname(std::string hostname);

		// Getters
		int			getSocket() const;
		bool		getIsRegistered() const;
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealname() const;
		std::string getHostname() const;
		Server		*getServer() const;

	private:
		int				_socketFd;
		bool			_isRegistered;

		std::string 	_nickname;
		std::string 	_username;
		std::string 	_realname;
		std::string 	_hostname;

		//std::vector<Channel *> _channels;
		Server *_server;
};

#endif