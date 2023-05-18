#include "Server.hpp"

// Constructeur

Server::Server(unsigned short port, std::string password)
	: _serverName(SERVERNAME),
	  _serverHostname(SERVERHOSTNAME),
	  _serverVersion(SERVERVERSION),
	  _port(port),
	  _serverPassword(password),
	  _serverCreationTime(std::time(NULL)),
	  _serverMotd("Welcome to the IRC server")
{
	//_iLastConnect = 0;
	initServer();
	initCommandHandlerMap();
}

// Destructeur

Server::~Server() {}

// Getters

const std::string Server::getServerName() const { return (_serverName); }

const std::string Server::getServerHostname() const { return (_serverHostname); }

const std::string Server::getServerInfo() const { return (_serverInfo); }

const std::string Server::getServerVersion() const { return (_serverVersion); }

const std::string Server::getServerEnvironment() const { return (_serverEnvironment); }

const std::string Server::getServerPassword() const { return (_serverPassword); }

time_t Server::getServerCreationTime() const { return (_serverCreationTime); }

const std::string Server::getServerMotd() const { return (_serverMotd); }

std::vector<Client *> Server::getClients() const { return (_clients); }

std::vector<Channel *> Server::getChannels() const { return (_channels); }

bool Server::isChannel(std::string channelName) const
{
	for (std::vector<Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getName() == channelName)
			return (true);
	}
	return (false);
}

Channel *Server::getChannel(std::string channelName) const
{
	for (std::vector<Channel *>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it)->getName() == channelName)
			return (*it);
	}
	return (NULL);
}

// Setters

void Server::setServerMotd(std::string motd) { _serverMotd = motd; }

// Client getters

Client *Server::getClient(int socketFd) const
{
	for (std::vector<Client *>::const_iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		if ((*it)->getSocket() == socketFd)
			return (*it);
	}
	return (NULL);
}

Client *Server::getClient(std::string nickname) const
{
	for (std::vector<Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return (*it);
	}
	return (NULL);
}

// Client add and remove

void Server::addClient(int clientSocket, struct sockaddr_in clientAddress)
{
	Client *newClient = new Client(this, clientSocket, clientAddress);

	if (newClient == NULL)
	{
		std::cerr << "Error: new client creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	_clients.push_back(newClient);
}

void Server::removeClient(Client *client)
{
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->getSocket(), NULL);
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it) == client)
		{
			_clients.erase(it);
			delete client;
			return;
		}
	}
	std::cout << "Client disconnected" << std::endl;
}

// Channel add and remove

void Server::addChannel(Channel *channel)
{
	_channels.push_back(channel);
}

void Server::removeChannel(Channel *channel)
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if ((*it) == channel)
		{
			_channels.erase(it);
			delete channel;
			return;
		}
	}
}

// Server initialization

void Server::initServer()
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

	if (bind(_serverSocket, (const struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)
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

	struct epoll_event ev;
	ev.data.fd = _serverSocket;
	ev.events = EPOLLIN | EPOLLET;

	if ((_epollFd = epoll_create(MAX_EVENTS)) < 0) // epoll_create1(0) ?
	{
		std::cerr << "Error: epoll creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Server socket epoll control

	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &ev) < 0)
	{
		std::cerr << "Error: epoll_ctl failed" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int Server::epollWait()
{
	// Server socket epoll wait
	int nfds = epoll_wait(_epollFd, _eventList, MAX_CLIENTS, -1);

	if (nfds < 0)
	{
		std::cerr << "Error: epoll_wait failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	return (nfds);
}

int Server::acceptNewClient()
{
	int newClientSocket;
	struct sockaddr_in newClientAddress;
	socklen_t newClientAddressLen = sizeof(newClientAddress);

	// Accept new client
	std::cout << "Accepting new client" << std::endl;
	if ((newClientSocket = accept(_serverSocket, (struct sockaddr *)&newClientAddress, (socklen_t *)&newClientAddressLen)) < 0)
	{
		std::cerr << "Error: accept failed - " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	addClient(newClientSocket, newClientAddress);

	return (newClientSocket);
}

// Loop du serveur
void Server::start()
{

	// Server socket epoll events loop
	while (125)
	{
		int nfds = this->epollWait();
		for (int i = 0; i < nfds; i++) // maybe store last i and start from it
		{
			if (_eventList[i].data.fd == _serverSocket) // new client
			{
				int newClientFd = this->acceptNewClient();
				if (fcntl(newClientFd, F_SETFL, O_NONBLOCK) < 0)
				{
					std::cerr << "Error: fnctl failed" << std::endl;
					// exit(EXIT_FAILURE); à reflechir
				}
				else
				{
					struct epoll_event ev;
					ev.data.fd = newClientFd;
					ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
					if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuseAddr, sizeof(_reuseAddr)) < 0)
					{
						std::cerr << "Error: setsockopt failed" << std::endl;
						// exit(EXIT_FAILURE); à reflechir
					}

					if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, newClientFd, &ev) < 0)
					{
						std::cerr << "Error: epoll_ctl failed" << std::endl;
						// exit(EXIT_FAILURE); à reflechir
					}
					//_iLastConnect = i;
				}
			}
			else if (_eventList[i].events & EPOLLIN) // read from client
			{
				Client *client = getClient(_eventList[i].data.fd);
				if (client == NULL)
				{
					std::cerr << "Error: client not found" << std::endl;
					// exit(EXIT_FAILURE); à reflechir
				}
				else
				{
					readFromClient(client);
				}
			}
			else // unknown event
			{
				std::cerr << "Error: unknown event" << std::endl;
				// exit(EXIT_FAILURE); à reflechir
			}
			if (_eventList[i].events & (EPOLLRDHUP | EPOLLHUP)) // client disconnected
			{
				Client *client = getClient(_eventList[i].data.fd);
				if (client == NULL)
				{
					std::cerr << "Error: client not found" << std::endl;
					// exit(EXIT_FAILURE); à reflechir
				}
				else
				{
					std::vector<std::string> args;
					args[0] = "connection lost with client";
					handleQuit(client, args);
					continue;
				}
			}
		}
	}
}