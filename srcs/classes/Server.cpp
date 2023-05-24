#include "Server.hpp"

extern bool run;

Server::Server(unsigned short port, std::string password)
	: _serverName(SERVERNAME), _serverHostname(SERVERHOSTNAME), _serverVersion(SERVERVERSION),
	  _port(port), _maxUsers(0), _serverPassword(password), _serverCreationTime(std::time(NULL))
{
	_commandHandlers["ADMIN"] = &Server::handleAdmin;
	_commandHandlers["INVITE"] = &Server::handleInvite;
	_commandHandlers["JOIN"] = &Server::handleJoin;
	_commandHandlers["KICK"] = &Server::handleKick;
	_commandHandlers["LIST"] = &Server::handleList;
	_commandHandlers["LUSERS"] = &Server::handleLusers;
	_commandHandlers["MODE"] = &Server::handleMode;
	_commandHandlers["MOTD"] = &Server::handleMotd;
	_commandHandlers["NAMES"] = &Server::handleNames;
	_commandHandlers["NICK"] = &Server::handleNick;
	_commandHandlers["OPER"] = &Server::handleOper;
	_commandHandlers["PART"] = &Server::handlePart;
	_commandHandlers["PASS"] = &Server::handlePass;
	_commandHandlers["PING"] = &Server::handlePing;
	_commandHandlers["PRIVMSG"] = &Server::handlePrivateMessage;
	_commandHandlers["QUIT"] = &Server::handleQuit;
	_commandHandlers["TIME"] = &Server::handleTime;
	_commandHandlers["TOPIC"] = &Server::handleTopic;
	_commandHandlers["USER"] = &Server::handleUser;
	_commandHandlers["USERHOST"] = &Server::handleUserhost;
	_commandHandlers["VERSION"] = &Server::handleVersion;
	_commandHandlers["WHO"] = &Server::handleWho;
	_commandHandlers["WHOIS"] = &Server::handleWhois;
	_commandHandlers["WHOWAS"] = &Server::handleWhowas;
}

Server::~Server()
{
	deleteVector(&_channels);
	deleteVector(&_clients);
	deleteVector(&_formerClients);
	close(_serverSocket);
	close(_epollFd);
}

const std::string Server::getServerName() const { return (_serverName); }

const std::string Server::getServerHostname() const { return (_serverHostname); }

const std::string Server::getServerInfo() const { return (_serverInfo); }

const std::string Server::getServerVersion() const { return (_serverVersion); }

const std::string Server::getServerPassword() const { return (_serverPassword); }

time_t Server::getServerCreationTime() const { return (_serverCreationTime); }

unsigned short Server::getPort() const { return (_port); }

std::vector<Client*> Server::getClients() const { return (_clients); }

std::vector<Channel*> Server::getChannels() const { return (_channels); }

Channel* Server::getChannel(std::string channelName) const
{
	for (std::vector<Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (toLowerCase((*it)->getName()) == toLowerCase(channelName))
			return (*it);
	}
	return (NULL);
}

Client* Server::getClient(int socketFd) const
{
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
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
		if (toLowerCase((*it)->getNickname()) == toLowerCase(nickname))
			return (*it);
	}
	return (NULL);
}

void Server::welcomeMessage(Client* client)
{
	client->setIsRegistered(true);
	client->setNickname(client->getNickname());
	handleLusers(client, std::vector<std::string>());
	handleMotd(client, std::vector<std::string>());
	std::cout << BLUE << *client << RESET << std::endl;
}

void Server::removeClient(Client* client)
{
	client->_message = "";

	epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->getSocket(), NULL);

	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it) == client)
		{
			_clients.erase(it);
			break;
		}
	}

	std::vector<Channel*> channels = client->getChannels();
	for (std::vector<Channel*>::reverse_iterator it = channels.rbegin(); it != channels.rend();
		 ++it)
		(*it)->removeClientFromChannel(client);

	client->markForDeletion();
}

void Server::addChannel(Channel* channel) { _channels.push_back(channel); }

void Server::removeChannel(Channel* channel)
{
	_channels.erase(std::find(_channels.begin(), _channels.end(), channel));
	_channelsToDelete.push_back(channel);
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
	syscall(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuseAddr, sizeof(_reuseAddr)),
			"setsockopt");
	syscall(bind(_serverSocket, (const struct sockaddr*)&_serverAddress, sizeof(_serverAddress)),
			"bind");
	syscall(listen(_serverSocket, BACKLOG), "listen");

	// Server socket epoll
	struct epoll_event ev;
	ev.data.fd = _serverSocket;
	ev.events = EPOLLIN | EPOLLET;
	syscall(_epollFd = epoll_create1(0), "epoll_create1");
	syscall(epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSocket, &ev), "epoll_ctl");
}

void Server::acceptNewClient()
{
	int				   newClientSocket;
	struct sockaddr_in newClientAddress;
	socklen_t		   newClientAddressLen = sizeof(newClientAddress);

	syscall(newClientSocket = accept(_serverSocket, (struct sockaddr*)&newClientAddress,
									 (socklen_t*)&newClientAddressLen),
			"accept");
	_clients.push_back(new Client(this, newClientSocket, newClientAddress));
	if (_clients.size() > _maxUsers)
		_maxUsers = _clients.size();
	syscall(fcntl(newClientSocket, F_SETFL, O_NONBLOCK), "fcntl");
	struct epoll_event ev;
	ev.data.fd = newClientSocket;
	ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
	syscall(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &_reuseAddr, sizeof(_reuseAddr)),
			"setsockopt");
	syscall(epoll_ctl(_epollFd, EPOLL_CTL_ADD, newClientSocket, &ev), "epoll_ctl");
	std::cout << BLUE << "Client " << newClientSocket << " connected." << RESET << std::endl;
}

void Server::loop()
{
	while (run)
	{
		int nfds = epoll_wait(_epollFd, _eventList, MAX_CLIENTS, -1);
		if (nfds < 0)
		{
			if (!run)
				return;
			throw SystemError("epoll_wait");
		}
		for (int i = 0; i < nfds; ++i)
		{
			if (_eventList[i].data.fd == _serverSocket)
			{
				this->acceptNewClient();
				continue;
			}
			Client* client = getClient(_eventList[i].data.fd);
			if (client != NULL)
			{
				if (_eventList[i].events & EPOLLIN)
					readFromClient(client);
				else if (_eventList[i].events & (EPOLLRDHUP | EPOLLHUP))
				{
					std::vector<std::string> args;
					args.push_back("Connection lost with client");
					handleQuit(client, args);
				}
				if (client->shouldBeDeleted())
					delete client;
			}
		}
		deleteVector(&_channelsToDelete);
	}
}
