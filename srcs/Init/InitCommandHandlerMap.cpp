#include "Server.hpp"

void Server::initCommandHandlerMap()
{
	_commandHandlers["CAP"] = &Server::handleCap;
	_commandHandlers["MODE"] = &Server::handleMode;
	_commandHandlers["MOTD"] = &Server::handleMotd;
	_commandHandlers["NICK"] = &Server::handleNick;
	_commandHandlers["PING"] = &Server::handlePing;  // A LAISSER EN ORDRE ALPHABETIQUE MERCI
	_commandHandlers["PRIVMSG"] = &Server::handlePrivateMessage;
	_commandHandlers["QUIT"] = &Server::handleQuit;
	_commandHandlers["USER"] = &Server::handleUser;
	_commandHandlers["WHOIS"] = &Server::handleWhois;
	// _commandHandlers["JOIN"] = &Server::handleJoin;
}