#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
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
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>

#define SERVERNAME "MiaoRC"
#define SERVERHOSTNAME "irc.125.outstanding.gov"
#define SERVERVERSION "0.021a"
#define INFO "42School"
#define BACKLOG 128 // why 128 ?
#define MAX_CLIENTS 1024
#define MAX_EVENTS 32 // why 10 ?
#define DEBUG true
#define BUFFER_SIZE 256
#define MAX_CHANNELS_PER_CLIENT 10
#define MAX_USERS_PER_CHANNEL 10

class Client;

class Channel;

class Server
{
	public:
		Server(short port, std::string password);
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
		std::vector<Channel> *getChannel() const;
		Channel *getChannel(std::string channelName) const;
		bool isChannel(std::string channelName) const;
		// 4. Vectors
		std::vector<Client *> getClients() const;
		std::vector<Channel *> getChannels() const;

		// Setters
		void setServerMotd(std::string motd);

		// Start
		void start();
		int epollWait();
		int acceptNewClient();

		// Methods
		// 1. Server
		void readFromClient(Client *client);
		void parseMessageFromClient(Client *client, std::string command);
		// 2. Client
		void addClient(int clientSocket, struct sockaddr_in clientAddress);
		void removeClient(Client *client);
		// 3. Channel
		void addChannel(Channel *channel);
		void removeChannel(Channel *channel);

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
		void handleJoin(Client *client, std::vector<std::string> arguments);
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
		void broadcastAll(std::string message);

		// Init
		void initServer();
		void initCommandHandlerMap();
		void initReplyMap();

	private:
		int _serverSocket;
		int _epollFd;
		int _newEvents;
		int _reuseAddr;
		int _iLastConnect;
		struct sockaddr_in _serverAddress; // Server address
		struct epoll_event _eventList[MAX_CLIENTS];
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
		std::vector<Client *>	_clients;
		std::vector<Channel *>	_channels;
};

#endif