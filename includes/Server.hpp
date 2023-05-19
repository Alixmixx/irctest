#pragma once

#include "ft_irc.hpp"

class Channel;
class Client;

class Server {
public:
	Server(unsigned short port, std::string password);
	~Server();
	void start();

	Channel*			  getChannel(std::string channelName) const;
	std::vector<Channel*> getChannels() const;
	Client*				  getClient(int socketFd) const;
	Client*				  getClient(std::string nickname) const;
	std::vector<Client*>  getClients() const;
	time_t				  getServerCreationTime() const;
	const std::string	  getServerHostname() const;
	const std::string	  getServerInfo() const;
	const std::string	  getServerMotd() const;
	const std::string	  getServerName() const;
	const std::string	  getServerPassword() const;
	const std::string	  getServerVersion() const;

	void readFromClient(Client* client);
	void parseMessageFromClient(Client* client, std::string command);
	void removeClient(Client* client);
	void addChannel(Channel* channel);
	void removeChannel(Channel* channel);

	void handleCap(Client* client, std::vector<std::string> arguments);
	void handleInvite(Client* client, std::vector<std::string> arguments);
	void handleJoin(Client* client, std::vector<std::string> arguments);
	void handleKick(Client* client, std::vector<std::string> arguments);
	void handleList(Client* client, std::vector<std::string> arguments);
	void handleMode(Client* client, std::vector<std::string> arguments);
	void handleMotd(Client* client, std::vector<std::string> arguments);
	void handleNames(Client* client, std::vector<std::string> arguments);
	void handleNick(Client* client, std::vector<std::string> arguments);
	void handlePart(Client* client, std::vector<std::string> arguments);
	void handlePing(Client* client, std::vector<std::string> arguments);
	void handlePrivateMessage(Client* client, std::vector<std::string> arguments);
	void handleQuit(Client* client, std::vector<std::string> arguments);
	void handleTopic(Client* client, std::vector<std::string> arguments);
	void handleUser(Client* client, std::vector<std::string> arguments);
	void handleWhois(Client* client, std::vector<std::string> arguments);

	// ServerCommands
	// void handleAdmin(Client *client, std::vector<std::string> arguments);
	// void handleInfo(Client *client, std::vector<std::string> arguments);
	// void handleTime(Client *client, std::vector<std::string> arguments);
	// void handleVersion(Client *client, std::vector<std::string> arguments);

	void broadcast(std::vector<Client*> recipients, std::string message);
	void broadcast(std::vector<Client*> recipients, std::string message, Client* except);
	void broadcast(std::vector<Client*> recipients, std::string message, std::vector<Client*> except);

private:
	int				   _serverSocket;
	int				   _epollFd;
	int				   _reuseAddr;
	int				   _iLastConnect;
	struct sockaddr_in _serverAddress;
	struct epoll_event _eventList[MAX_CLIENTS];
	const std::string  _serverName;
	const std::string  _serverHostname;
	const std::string  _serverInfo;
	const std::string  _serverVersion;
	unsigned short	   _port;

	typedef void (Server::*CommandHandler)(Client* client, std::vector<std::string> arguments);
	std::map<std::string, CommandHandler> _commandHandlers;

	const std::string _serverPassword;
	const time_t	  _serverCreationTime;
	std::string		  _serverMotd;

	std::vector<Client*>  _clients;
	std::vector<Channel*> _channels;

	void acceptNewClient();
	void init();
	void loop();
};
