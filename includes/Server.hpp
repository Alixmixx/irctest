#pragma once

#include "ft_irc.hpp"

class Channel;
class Client;

class Server {
public:
	Server(unsigned short port, std::string password);
	~Server();
	void start();

	void addChannel(Channel* channel);

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

private:
	int				   _serverSocket;
	int				   _epollFd;
	int				   _reuseAddr;
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

	void broadcast(std::vector<Client*> recipients, std::string message);
	void broadcast(std::vector<Client*> recipients, std::string message, Client* except);
	void broadcast(std::vector<Client*> recipients, std::string message, std::vector<Client*> except);

	void readFromClient(Client* client);
	void parseMessageFromClient(Client* client, std::string command);
	void removeClient(Client* client);
	void removeChannel(Channel* channel);

	void acceptNewClient();
	void init();
	void loop();
};
