#include "Server.hpp"

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

bool Server::isChannel(std::string channelName) const
{
	for (std::vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getName() == channelName)
			return (true);
	}
	return (false);
}

Channel* Server::getChannel(std::string channelName) const
{
	for (std::vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getName() == channelName)
			return (*it);
	}
	return (NULL);
}

void Server::setServerMotd(std::string motd) { _serverMotd = motd; }

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

void Server::addClient(int clientSocket, struct sockaddr_in clientAddress)
{
	Client* newClient = new Client(this, clientSocket, clientAddress);

	if (newClient == NULL)
	{
		std::cerr << "Error: new client creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	_clients.push_back(newClient);
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
			delete client;
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
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it) == channel)
		{
			_channels.erase(it);
			delete channel;
			return;
		}
	}
}

void Server::init()
{
	// Server address initialization
	std::memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	_serverAddress.sin_port = htons(_port);

	// Server socket creation
	if ((_serverSocket = socket(_serverAddress.sin_family, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set the server socket to allow multiple non-blocking connections
	fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

	// Server socket binding
	_reuseAddr = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuseAddr, sizeof(_reuseAddr)) < 0)
	{
		std::cerr << "Error: setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (bind(_serverSocket, (const struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0)
	{
		std::cerr << "Error: socket binding failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Server socket listening
	if (listen(_serverSocket, BACKLOG) < 0)
	{
		std::cerr << "Error: socket listening failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Server socket epoll creation
	if ((_epollFd = epoll_create(MAX_EVENTS)) < 0) // epoll_create1(0) ?
	{
		std::cerr << "Error: epoll creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Server socket epoll control
	struct epoll_event ev;
	ev.data.fd = _serverSocket;
	ev.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &ev) < 0)
	{
		std::cerr << "Error: epoll_ctl failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << BLUE << "Listening on port " << _port << ". 👂" << RESET << std::endl;
}

int Server::acceptNewClient()
{
	int				   newClientSocket;
	struct sockaddr_in newClientAddress;
	socklen_t		   newClientAddressLen = sizeof(newClientAddress);

	std::cout << BLUE << "Client connected." << RESET << std::endl;
	if ((newClientSocket = accept(_serverSocket, (struct sockaddr*)&newClientAddress, (socklen_t*)&newClientAddressLen)) < 0)
	{
		std::cerr << "Error: accept failed - " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	addClient(newClientSocket, newClientAddress);
	return (newClientSocket);
}

void Server::loop()
{
	int nfds;
	while (true)
	{
		syscall(nfds = epoll_wait(_epollFd, _eventList, MAX_CLIENTS, -1), "epoll_wait");
		for (int i = 0; i < nfds; ++i)
		{
			if (_eventList[i].data.fd == _serverSocket)
			{
				int newClientFd = this->acceptNewClient();
				syscall(fcntl(newClientFd, F_SETFL, O_NONBLOCK), "fcntl");
				struct epoll_event ev;
				ev.data.fd = newClientFd;
				ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
				syscall(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuseAddr, sizeof(_reuseAddr)), "setsockopt");
				syscall(epoll_ctl(_epollFd, EPOLL_CTL_ADD, newClientFd, &ev), "epoll_ctl");
				continue ;
			}
			Client* client = getClient(_eventList[i].data.fd);
			if (client == NULL)
				panic("Unknown client: " + toString(_eventList[i].data.fd) + ".");
			if (_eventList[i].events & EPOLLIN)
				readFromClient(client);
			else if (_eventList[i].events & (EPOLLRDHUP | EPOLLHUP))
			{
				std::vector<std::string> args; // TODO c'est la merde
				args.push_back("connection lost with client");
				handleQuit(client, args);
			}
			else
				panic("Unknown event: " + toString(_eventList[i].events) + ".");
		}
	}
}

void Server::start()
{
	this->init();
	this->loop();
}