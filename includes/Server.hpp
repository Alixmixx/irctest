#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "ReplyCode.hpp"

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
		const std::string getServerHostname() const;
		const std::string getServerInfo() const;
		const std::string getServerVersion() const;
		const std::string getServerEnvironment() const;
		const std::string getServerMotd() const;
		// 2. Client
		Client *getClient(int socketFd) const;
		Client *getClient(std::string nickname) const;
		// 3. Channel
		// std::vector<Channel> *getChannel() const;
		// 4. Vectors
		std::vector<Client *> getClients() const;
		// std::vector<Channel *> getChannels() const;

		// Setters
		void setServerMotd(std::string motd);

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
		// 1. Server
		void readFromClient(Client *client);
		void parseMessageFromClient(Client *client, std::string command);
		// 2. Client
		void addClient(int socketFd);
		void removeClient(Client *client);
		// 3. Channel
		// void addChannel(Channel channel);
		// void removeChannel(Channel channel);

		// Commands

		// 1. ConnectionCommands
		void handleCap(Client *client, std::vector<std::string> arguments);
		void handleMode(Client *client, std::vector<std::string> arguments);
		void handleNick(Client *client, std::vector<std::string> arguments);  // A LAISSER EN ORDRE ALPHABETIQUE MERCI
		void handlePing(Client *client, std::vector<std::string> arguments);
		void handleQuit(Client *client, std::vector<std::string> arguments);
		void handleUser(Client *client, std::vector<std::string> arguments);
		void handleWhois(Client *client, std::vector<std::string> arguments);
		// 2. ChannelCommands
		// void handleJoin(Client *client, std::vector<std::string> arguments);
		// void handleKick(Client *client, std::vector<std::string> arguments);
		// void handleList(Client *client, std::vector<std::string> arguments);
		// void handleNames(Client *client, std::vector<std::string> arguments);
		// void handlePart(Client *client, std::vector<std::string> arguments);
		// void handleTopic(Client *client, std::vector<std::string> arguments);
		// 3. ServerCommands
		/* void handleAdmin(Client *client, std::vector<std::string> arguments);
		void handleInfo(Client *client, std::vector<std::string> arguments);
		void handleTime(Client *client, std::vector<std::string> arguments);
		void handleVersion(Client *client, std::vector<std::string> arguments); */
		// 4. SendingMessage
		void handlePrivateMessage(Client *client, std::vector<std::string> arguments);
		void handleMotd(Client *client, std::vector<std::string> arguments);

		// Message (server to client)
		void replyMessage(Client *client, std::string replyCode);
		void replyMessage(Client *client, std::string replyCode, std::string arg1);
		void replyMessage(Client *client, std::string replyCode, std::string arg1, std::string arg2);
		void replyMessage(Client *client, std::string replyCode, std::string arg1, std::string arg2, std::string arg3);

		// Init
		void initCommandHandlerMap();
		void initReplyMap();

	private:
		int _epollFd;
		int _socketFd;
		struct epoll_event _evlist[MAX_CLIENTS];
		const std::string _serverName;
		const std::string _serverHostname;
		const std::string _serverInfo;
		const std::string _serverVersion;
		const std::string _serverEnvironment;
		short _port;

		typedef void (Server::*CommandHandler)(Client *client, std::vector<std::string> arguments);
		std::map<std::string, CommandHandler> _commandHandlers;
		std::map<std::string, ReplyCode> _replyCodeMap;

		// Server infos
		const std::string	_serverPassword;
		const std::string	_serverCreationDate;
		const std::string	_serverCreationTime;
		std::string    	 	_serverMotd;	// Message of the day

		// Server vector and map
		std::vector<Client *> _clients;
		// std::vector<Channel>	_channels;
};

#endif