#include "Server.hpp"
#include "Client.hpp"

// Constructeur

Client::Client(Server *server, int socketFd, std::string hostname)
	: _socketFd(socketFd),
	  _isRegistered(false),
	  _nickname(""),
	  _hostname(hostname),
	  _server(server)
{
}
// Destructeur
Client::~Client()
{
}

// Setters

void Client::setNickname(std::string nickname) { _nickname = nickname; }

void Client::setIsRegistered(bool isRegistered) { _isRegistered = isRegistered; }

void Client::setUsername(std::string username) { _username = username; }

void Client::setRealname(std::string realname) { _realname = realname; }

void Client::setHostname(std::string hostname) { _hostname = hostname; }

// Getters

int Client::getSocket() const { return (_socketFd); }

bool Client::getIsRegistered() const { return (_isRegistered); }

std::string Client::getNickname() const { return (_nickname); }

std::string Client::getUsername() const { return (_username); }

std::string Client::getRealname() const { return (_realname); }

std::string Client::getHostname() const { return (_hostname); }

Server *Client::getServer() const { return (_server); }
