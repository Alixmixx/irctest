#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>

#define BACKLOG 128
#define MAX_CLIENTS 1024
#define BUFFER_SIZE_IRC 1024
#define DEBUG true

class Client;

class Server
{
	public:
		Server(std::string serverName, std::string serverInfo, std::string serverVersion, std::string serverEnvironment, std::string port);
		~Server();

		// Getters
		// 1. Server
		const std::string getServerName() const;
		const std::string getServerInfo() const;
		const std::string getServerVersion() const;
		const std::string getServerEnvironment() const;
		// 2. Vectors
		std::vector<Client *> getClients() const;
		std::map<std::string, std::string> getErrorMap() const;
		std::map<std::string, std::string> getReplyMap() const;
		// 3. Client
		Client *getClient(int socketFd) const;
		Client *getClient(std::string nickname) const;
		// 4. Channel
		// std::vector<Channel> *getChannels() const;

		// Statics
		void clean();
		void goodBye();
		void handleSigint(int signum);
		void syscall(int returnValue, const char *funcName);
		std::string fullRead(int fd);
		void addEvent(int epollFd, int eventFd);
		void initEpoll(char *port);
		void loop();

		// Methods
		void addClient(int socketFd);
		void removeClient(Client client);

		// Read
		void readFromClient(Client *client);
		void parseMessageFromClient(Client *client, std::string command);

		// void addChannel(Channel channel);
		// void removeChannel(Channel channel);


		// Commands

		// 1. ClientCommands
		void handleCap(Client *client, std::vector<std::string> arguments);
		void handleMode(Client *client, std::vector<std::string> arguments);
		void handleNick(Client *client, std::vector<std::string> arguments);
		void handleUser(Client *client, std::vector<std::string> arguments);


		// Message
		void sendReplyMessage(Client *client, std::string replyMessage);
		void sendErrorMessage(Client *client, std::string errorMessage); // faudra les merge, je crois que c'est la meme chose

		// Init
		void initCommandHandlerMap();
		void initErrorMap();
		void initReplyMap(); // error map et reply map c'est aussi la meme chose

	private:
		int _epollFd;
		int _socketFd;
		struct epoll_event _evlist[MAX_CLIENTS];
		const std::string _serverName;
		const std::string _serverInfo;
		const std::string _serverVersion;
		const std::string _serverEnvironment;
		short _port;

		typedef void (Server::*CommandHandler)(Client *client, std::vector<std::string> arguments);
		std::map<std::string, CommandHandler> _commandHandlers;

		// Server infos
		const std::string _serverPassword;
		const std::string _serverCreationDate;
		const std::string _serverCreationTime;

		// Server vector and map
		std::vector<Client *> _clients;
		// std::vector<Channel>	_channels;
		std::map<std::string, std::string> _errorMap;
		std::map<std::string, std::string> _replyMap;
};

#endif