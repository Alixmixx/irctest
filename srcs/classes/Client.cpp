#include "Client.hpp"
#include "Server.hpp"

Client::Client(Server* server, int socketFd, sockaddr_in clientAddress)
	: _clientSocket(socketFd), _modes(0), _isRegistered(false), _isInvisible(false),
	  _isPasswordCorrect(false), _clientAddress(clientAddress), _server(server)
{
	time_t t0 = std::time(NULL);
	_signonTime = t0;
	_lastAction = t0;
}

Client::~Client()
{
	close(_clientSocket);
	std::cout << BLUE << "Client " << _clientSocket << " disconnected." << RESET << std::endl;
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

void Client::setIsInvisible(bool invisible) { _isInvisible = invisible; }

void Client::setLastAction() { _lastAction = std::time(NULL); }

void Client::setPasswordCorrect() { _isPasswordCorrect = true; }

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

int Client::getSocket() const { return _clientSocket; }

bool Client::isRegistered() const { return _isRegistered; }

sockaddr_in Client::getClientAddress() const { return _clientAddress; }

std::string Client::getNickname() const { return _nickname; }

std::string Client::getUsername() const { return _username; }

std::string Client::getRealname() const { return _realname; }

std::string Client::getHostname() const { return _hostname; }

std::string Client::getPrefix() const { return _prefix; }

std::string Client::getIp() const
{
	unsigned long ip = _clientAddress.sin_addr.s_addr;
	return toString(ip & 255) + "." + toString(ip >> 8 & 255) + "." + toString(ip >> 16 & 255) +
		   "." + toString(ip >> 24 & 255);
}

std::string Client::getModeString() const
{
	std::string modeString = "+";
	if (_isInvisible)
		modeString += "i";
	return modeString;
}

bool Client::isInvisible() const { return _isInvisible; }

bool Client::isPasswordCorrect() const { return _isPasswordCorrect; }

Server* Client::getServer() const { return _server; }

std::vector<Channel*> Client::getChannels() const { return _channels; };

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