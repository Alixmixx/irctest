#include "Server.hpp"
#include "Client.hpp"

// Constructeur

Client::Client(Server *server, int socketFd, struct sockaddr_in clientAddress)
	: _clientSocket(socketFd),
	  _isRegistered(false),
	  _clientAddress(clientAddress),
	  _nickname(""),
	  _username(""),
	  _realname(""),
	  _hostname(""),
	  _server(server)
{
	(void)_clientAddress;
}

// Destructeur
Client::~Client()
{
	close(_clientSocket);
}

// Setters

void Client::setNickname(std::string nickname) { _nickname = nickname; }

void Client::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }

void Client::setUsername(std::string username) { _username = username; }

void Client::setRealname(std::string realname) { _realname = realname; }

void Client::setHostname(std::string hostname) { _hostname = hostname; }

void Client::setMessage(std::string message) { _message = message; }

void Client::setChannelCount(int channelCount) { _channelCount = channelCount; }

// Getters

int Client::getSocket() const { return (_clientSocket); }

int Client::getChannelCount() const { return (_channelCount); }

bool Client::getIsRegistered() const { return (_isRegistered); }

std::string Client::getNickname() const { return (_nickname); }

std::string Client::getUsername() const { return (_username); }

std::string Client::getRealname() const { return (_realname); }

std::string Client::getHostname() const { return (_hostname); }

std::string Client::getMessage() const { return (_message); }

Server *Client::getServer() const { return (_server); }
