#include "Server.hpp"

void Server::initCommandHandlerMap()
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
	_commandHandlers["PING"] = &Server::handlePing;  // A LAISSER EN ORDRE ALPHABETIQUE MERCI
	_commandHandlers["PRIVMSG"] = &Server::handlePrivateMessage;
	_commandHandlers["QUIT"] = &Server::handleQuit;
	_commandHandlers["TOPIC"] = &Server::handleTopic;
	_commandHandlers["USER"] = &Server::handleUser;
	_commandHandlers["WHOIS"] = &Server::handleWhois;
}
