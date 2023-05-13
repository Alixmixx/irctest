#include "Server.hpp"

void Server::initCommandHandlerMap()
{
	_commandHandlers["NICK"] = &Server::handleNick;
	_commandHandlers["CAP"] = &Server::handleCap;
	_commandHandlers["USER"] = &Server::handleUser;
	// _commandHandlers["QUIT"] = &Server::handleQuit;
	// _commandHandlers["JOIN"] = &Server::handleJoin;
}