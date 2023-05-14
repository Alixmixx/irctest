#include "Server.hpp"

// peut etre merge replies et error replies ??
void Server::initReplyMap()
{
	_replyMap["RPL_WELCOME"] = "001 :Welcome to the <networkname> Network, <nick>[!<user>@<host>]";
	_replyMap["RPL_YOURHOST"] = "002 :Your host is <servername>, running version <version>";
	_replyMap["RPL_CREATED"] = "003 :This server was created <date> <time>";
	_replyMap["RPL_MYINFO"] = "004 <servername> <version> <available user modes> <available channel modes>";
	_replyMap["RPL_ISUPPORT"] = "005 <1-13 tokens>  :are supported by this server"; // <<<---- faut que je lise la doc, j'ai rien comps

	//... ajouter les autres replies EN ORDRE DE CODE!!!!, pour l'instant c'est que le handshake initial
}