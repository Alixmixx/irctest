#include "Server.hpp"

// peut etre merge replies et error replies ??
void Server::initReplyMap()
{
	_replyMap.insert(std::make_pair("RPL_WELCOME", "001 :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"));
	_replyMap.insert(std::make_pair("RPL_YOURHOST", "002 :Your host is <servername>, running version <version>"));
	_replyMap.insert(std::make_pair("RPL_CREATED", "003 :This server was created <datetime>"));
	_replyMap.insert(std::make_pair("RPL_MYINFO", "004 <servername> <version> <available user modes> <available channel modes>"));
	_replyMap.insert(std::make_pair("RPL_ISUPPORT", "005 <1-13 tokens>  :are supported by this server")); // <<<---- faut que je lise la doc, j'ai rien compris

	//... ajouter les autres replies EN ORDRE DE CODE!!!!, pour l'instant c'est que le handshake initial
}