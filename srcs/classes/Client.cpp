#include "Client.hpp"
#include "Server.hpp"

Client::Client(Server* server, int socketFd, sockaddr_in clientAddress)
	: _clientSocket(socketFd),
	  _isRegistered(false),
	  _isInvisible(false),
	  _clientAddress(clientAddress),
	  _server(server)
{
	time_t t0 = std::time(NULL);
	_signonTime = t0;
	_lastAction = t0;
}

Client::~Client()
{
	std::vector<Channel*>::iterator it = _channels.begin();
	for (; it != _channels.end(); ++it)
	{
		(*it)->removeClientFromChannel(this);
	}

	close(_clientSocket);
}

// Setters

void Client::setNickname(std::string nickname)
{
	_nickname = nickname;
	_prefix = ":" + _nickname + "!" + _username + "@" + _hostname;
}

void Client::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }

void Client::setUsername(std::string username) { _username = username; }

void Client::setRealname(std::string realname) { _realname = realname; }

void Client::setHostname(std::string hostname) { _hostname = hostname; }

void Client::setMessage(std::string message) { _message = message; }

void Client::setIsInvisible(bool invisible) { _isInvisible = invisible; }

void Client::setLastAction() { _lastAction = std::time(NULL); }

void Client::addChannel(Channel* channel) { _channels.push_back(channel); }

void Client::leaveChannel(Channel* channel)
{
	std::vector<Channel*>::iterator it = _channels.begin();
	for (; it != _channels.end(); ++it)
	{
		if ((*it) == channel)
		{
			_channels.erase(it);
			break;
		}
	}
}

// Getters

int Client::getSocket() const { return (_clientSocket); }

bool Client::isRegistered() const { return (_isRegistered); }

sockaddr_in Client::getClientAddress() const { return (_clientAddress); }

std::string Client::getNickname() const { return (_nickname); }

std::string Client::getUsername() const { return (_username); }

std::string Client::getRealname() const { return (_realname); }

std::string Client::getHostname() const { return (_hostname); }

std::string Client::getPrefix() const { return (_prefix); }

std::string Client::getMessage() const { return (_message); }

bool Client::isInvisible() const { return (_isInvisible); }

Server* Client::getServer() const { return (_server); }

std::vector<Channel*> Client::getChannels() const { return (_channels); };

time_t Client::getSignonTime() const { return _signonTime; };

time_t Client::getLastAction() const { return _lastAction; };

std::ostream& operator<<(std::ostream& os, const Client& client)
{
	os << "Client(nickname=" << client.getNickname() << ", "
	   << "username=" << client.getUsername() << ", "
	   << "hostname=" << client.getHostname() << ", "
	   << "realname=" << client.getRealname() << ")";
	return os;
}