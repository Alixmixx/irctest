#include "Server.hpp"

extern bool run;

Server::Server(unsigned short port, std::string password)
	: _serverName(SERVERNAME),
	  _serverHostname(SERVERHOSTNAME),
	  _serverVersion(SERVERVERSION),
	  _port(port),
	  _serverPassword(password),
	  _serverCreationTime(std::time(NULL)),
	  _serverMotd(MOTD)
{
	_commandHandlers["CAP"] = &Server::handleCap;
	_commandHandlers["INVITE"] = &Server::handleInvite;
	_commandHandlers["JOIN"] = &Server::handleJoin;
	_commandHandlers["KICK"] = &Server::handleKick;
	_commandHandlers["LIST"] = &Server::handleList;
	_commandHandlers["MODE"] = &Server::handleMode;
	_commandHandlers["MOTD"] = &Server::handleMotd;
	_commandHandlers["NAMES"] = &Server::handleNames;
	_commandHandlers["NICK"] = &Server::handleNick;
	_commandHandlers["PART"] = &Server::handlePart;
	_commandHandlers["PING"] = &Server::handlePing;
	_commandHandlers["PRIVMSG"] = &Server::handlePrivateMessage;
	_commandHandlers["QUIT"] = &Server::handleQuit;
	_commandHandlers["TOPIC"] = &Server::handleTopic;
	_commandHandlers["USER"] = &Server::handleUser;
	_commandHandlers["WHOIS"] = &Server::handleWhois;
}

Server::~Server() {}

const std::string Server::getServerName() const { return (_serverName); }

const std::string Server::getServerHostname() const { return (_serverHostname); }

const std::string Server::getServerInfo() const { return (_serverInfo); }

const std::string Server::getServerVersion() const { return (_serverVersion); }

const std::string Server::getServerPassword() const { return (_serverPassword); }

time_t Server::getServerCreationTime() const { return (_serverCreationTime); }

const std::string Server::getServerMotd() const { return (_serverMotd); }

std::vector<Client*> Server::getClients() const { return (_clients); }

std::vector<Channel*> Server::getChannels() const { return (_channels); }

Channel* Server::getChannel(std::string channelName) const
{
	for (std::vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getName() == channelName)
			return (*it);
	}
	return (NULL);
}

Client* Server::getClient(int socketFd) const
{
	for (std::vector<Client*>::const_iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		if ((*it)->getSocket() == socketFd)
			return (*it);
	}
	return (NULL);
}

Client* Server::getClient(std::string nickname) const
{
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return (*it);
	}
	return (NULL);
}

void Server::removeClient(Client* client)
{
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->getSocket(), NULL);
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it) == client)
		{
			_clients.erase(it);
			std::cout << BLUE << "Client disconnected." << RESET << std::endl;
			return;
		}
	}
}

void Server::addChannel(Channel* channel)
{
	_channels.push_back(channel);
}

void Server::removeChannel(Channel* channel)
{
	_channels.erase(std::find(_channels.begin(), _channels.end(), channel));
	delete channel;
}

void Server::init()
{
	// Server address initialization
	std::memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	_serverAddress.sin_port = htons(_port);

	// Server socket creation
	syscall(_serverSocket = socket(_serverAddress.sin_family, SOCK_STREAM, 0), "socket");
	fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

	// Server socket binding
	_reuseAddr = 1;
	syscall(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuseAddr, sizeof(_reuseAddr)), "setsockopt");
	syscall(bind(_serverSocket, (const struct sockaddr*)&_serverAddress, sizeof(_serverAddress)), "bind");
	syscall(listen(_serverSocket, BACKLOG), "listen");

	// Server socket epoll
	struct epoll_event ev;
	ev.data.fd = _serverSocket;
	ev.events = EPOLLIN | EPOLLET;
	syscall(_epollFd = epoll_create1(0), "epoll_create1");
	syscall(epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &ev), "epoll_ctl");

	std::cout << BLUE << "Listening on port " << _port << ". 👂" << RESET << std::endl;
}

void Server::acceptNewClient()
{
	int				   newClientSocket;
	struct sockaddr_in newClientAddress;
	socklen_t		   newClientAddressLen = sizeof(newClientAddress);

	std::cout << BLUE << "Client connected." << RESET << std::endl;
	syscall(newClientSocket = accept(_serverSocket, (struct sockaddr*)&newClientAddress, (socklen_t*)&newClientAddressLen), "accept");
	_clients.push_back(new Client(this, newClientSocket, newClientAddress));
	syscall(fcntl(newClientSocket, F_SETFL, O_NONBLOCK), "fcntl");
	struct epoll_event ev;
	ev.data.fd = newClientSocket;
	ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
	syscall(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuseAddr, sizeof(_reuseAddr)), "setsockopt");
	syscall(epoll_ctl(_epollFd, EPOLL_CTL_ADD, newClientSocket, &ev), "epoll_ctl");
}

void Server::loop()
{
	while (run)
	{
		int nfds = epoll_wait(_epollFd, _eventList, MAX_CLIENTS, -1);
		if (nfds < 0)
			return;
		for (int i = 0; i < nfds; ++i)
		{
			if (_eventList[i].data.fd == _serverSocket)
			{
				this->acceptNewClient();
				continue;
			}
			Client* client = getClient(_eventList[i].data.fd);
			if (client == NULL)
				panic("Unknown client: " + toString(_eventList[i].data.fd) + ".");
			if (_eventList[i].events & EPOLLIN)
				readFromClient(client);
			else if (_eventList[i].events & (EPOLLRDHUP | EPOLLHUP))
			{
				std::vector<std::string> args;
				args.push_back("connection lost with client");
				handleQuit(client, args);
			}
			else
				panic("Unknown event: " + toString(_eventList[i].events) + " on client " + toString(_eventList[i].data.fd) + ".");
		}
	}
}

void Server::start()
{
	this->init();
	this->loop();
}