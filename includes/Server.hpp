#pragma once

#include "ft_irc.hpp"

class Bot;

class Server {
public:
	Server(short port, std::string password);
	~Server();

	void init();
	void loop();

	void addChannel(Channel* channel);

	Channel*			  getChannel(std::string channelName) const;
	std::vector<Channel*> getChannels() const;
	Client*				  getClient(int socketFd) const;
	Client*				  getClient(std::string nickname) const;
	std::vector<Client*>  getClients() const;
	short				  getPort() const;
	time_t				  getServerCreationTime() const;
	const std::string	  getServerHostname() const;
	const std::string	  getServerInfo() const;
	const std::string	  getServerName() const;
	const std::string	  getServerPassword() const;
	const std::string	  getServerVersion() const;

	void removeChannel(Channel* channel);
	void handleMode(Client* client, std::vector<std::string> arguments);

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
	std::string		   _motd;
	time_t			   _motdCreationTime;
	short				_port;
	unsigned int	   _maxUsers;

	const std::string _serverPassword;
	const time_t	  _serverCreationTime;
	Bot*			  _bot;

	std::vector<Client*>	   _clients;
	std::vector<Client*>	   _clientsToDelete;
	std::vector<FormerClient*> _formerClients;
	std::vector<Channel*>	   _channels;
	std::vector<Channel*>	   _channelsToDelete;

	typedef void (Server::*CommandHandler)(Client*, std::vector<std::string>);
	std::map<std::string, CommandHandler> _commandHandlers;

	void welcomeMessage(Client* client);

	void handleAdmin(Client* client, std::vector<std::string> arguments);
	void handleInvite(Client* client, std::vector<std::string> arguments);
	void handleJoin(Client* client, std::vector<std::string> arguments);
	void handleKick(Client* client, std::vector<std::string> arguments);
	void handleList(Client* client, std::vector<std::string> arguments);
	void handleLusers(Client* client, std::vector<std::string> arguments);
	void handleMotd(Client* client, std::vector<std::string> arguments);
	void handleNames(Client* client, std::vector<std::string> arguments);
	void handleNick(Client* client, std::vector<std::string> arguments);
	void handleOper(Client* client, std::vector<std::string> arguments);
	void handlePart(Client* client, std::vector<std::string> arguments);
	void handlePass(Client* client, std::vector<std::string> arguments);
	void handlePing(Client* client, std::vector<std::string> arguments);
	void handlePrivateMessage(Client* client, std::vector<std::string> arguments);
	void handleQuit(Client* client, std::vector<std::string> arguments);
	void handleTime(Client* client, std::vector<std::string> arguments);
	void handleTopic(Client* client, std::vector<std::string> arguments);
	void handleUser(Client* client, std::vector<std::string> arguments);
	void handleUserhost(Client* client, std::vector<std::string> arguments);
	void handleVersion(Client* client, std::vector<std::string> arguments);
	void handleWho(Client* client, std::vector<std::string> arguments);
	void handleWhois(Client* client, std::vector<std::string> arguments);
	void handleWhowas(Client* client, std::vector<std::string> arguments);

	void broadcast(std::vector<Client*> recipients, std::string message, Client* except = NULL);

	void addClientToChannel(Client* client, Channel* channel, std::string channelPassword);
	void newChannel(Client* client, std::string channelName, std::string channelPassword);
	void channelWelcomeMessage(Client* client, Channel* channel);
	void acceptNewClient();
	void readFromClient(Client* client);
	void parseMessageFromClient(Client* client, std::string command);
	void removeClient(Client* client);

	void setModeClient(Client* client, std::vector<std::string> arguments);
	void setModeChannel(Client* client, Channel* channel, std::vector<std::string> arguments);
};
