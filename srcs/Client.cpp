#include "Server.hpp"
#include "Client.hpp"

// Constructeur

Client::Client(int socket, std::string nickname, std::string username, std::string realname, std::string hostname)
	:_socket(socket),
	_nickname(nickname),
	_username(username),
	_realname(realname),
	_hostname(hostname)
{}

// Destructeur
Client::~Client()
{}