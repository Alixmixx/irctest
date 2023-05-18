#include "Client.hpp"

void Client::reply(ReplyCode replyCode)
{
	std::string replyMessage = ":" + _server->getServerHostname() + " ";

	if (replyCode == RPL_WELCOME)
		replyMessage += "001 :Welcome to the <networkname> Network, " + _nickname + "[!" + _username + "@" + _hostname + "]";
	else if (replyCode == RPL_YOURHOST)
		replyMessage += "002 :Your host is " + _server->getServerName() + ", running version " + _server->getServerVersion();
	else if (replyCode == RPL_CREATED)
		replyMessage += "003 :This server was created " + toString(_server->getServerCreationDateTime());
	else if (replyCode == RPL_MYINFO)
		replyMessage += "004 :" + _server->getServerName() + " " + _server->getServerVersion() + " <available user modes> <available channel modes>";
	else if (replyCode == RPL_ISUPPORT)
		replyMessage += "005 :"; //"005 :<1-13 tokens>  :are supported by this server";
	else if (replyCode == RPL_UMODEIS)
		replyMessage += "221 :<user modes>"; // Client.getMode(); <<-- creer une fonction getMode dans client apres avoir lu la doc
	else if (replyCode == RPL_STATSUPTIME)
		replyMessage += "242 :Server Up <days> days <hours>:<minutes>:<seconds>"; // + getServerUptime(); <<-- creer une fonction getServerUptime qui retourne le temps depuis le lancement du serv
	else if (replyCode == RPL_LISTSTART)
		replyMessage += "321 :Channel :Users Name";
	else if (replyCode == RPL_LISTEND)
		replyMessage += "323 :End of /LIST";
	else if (replyCode == ERR_NOMOTD)
		replyMessage += "422 :MOTD File is missing";
	else if (replyCode == ERR_NOORIGIN)
		replyMessage += "409 :No origin specified";
	else if (replyCode == ERR_NOTEXTTOSEND)
		replyMessage += "412 :No text to send";
	else if (replyCode == ERR_NONICKNAMEGIVEN)
		replyMessage += "431 :No nickname given";
	else if (replyCode == ERR_SUMMONDISABLED)
		replyMessage += "445 :SUMMON has been disabled";
	else if (replyCode == ERR_USERSDISABLED)
		replyMessage += "446 :USERS has been disabled";
	else if (replyCode == ERR_NOTREGISTERED)
		replyMessage += "451 :You have not registered";
	else if (replyCode == ERR_ALREADYREGISTERED)
		replyMessage += "462 :You may not reregister";
	else if (replyCode == ERR_NOPERMFORHOST)
		replyMessage += "463 :Your host isn't among the privileged";
	else if (replyCode == ERR_PASSWDMISMATCH)
		replyMessage += "464 :Password incorrect";
	else if (replyCode == ERR_YOUREBANNEDCREEP)
		replyMessage += "465 :You are banned from this server";
	else if (replyCode == ERR_NOPRIVILEGES)
		replyMessage += "481 :Permission Denied- You're not an IRC operator";
	else if (replyCode == ERR_CANTKILLSERVER)
		replyMessage += "483 :You cant kill a server!";
	else if (replyCode == ERR_NOOPERHOST)
		replyMessage += "491 :No O-lines for your host";
	else if (replyCode == ERR_UMODEUNKNOWNFLAG)
		replyMessage += "501 :Unknown MODE flag";
	else if (replyCode == ERR_USERSDONTMATCH)
		replyMessage += "502 :Cant change mode for other users";

	replyMessage += "\r\n";

	replyMessage.insert(replyMessage.find(':', replyMessage.find(':') + 1), _nickname + " ");

	if (DEBUG)
		std::cout << "\033[1;32mMessage to client: " << _clientSocket << "\n"
				  << replyMessage << "\033[0m" << std::endl;

	send(_clientSocket, replyMessage.c_str(), replyMessage.length(), 0);
}

void Client::reply(ReplyCode replyCode, std::string arg1)
{
	std::string replyMessage = ":" + _server->getServerHostname() + " ";
	int rplCode = 1;

	if (replyCode == RPL_ENDOFWHOIS)
		replyMessage += "318 :" + arg1;
	else if (replyCode == RPL_NOTOPIC)
		replyMessage += "331 :" + arg1;
	else if (replyCode == RPL_ENDOFNAMES)
		replyMessage += "366 :" + arg1;
	else if (replyCode == ERR_NOSUCHNICK)
		replyMessage += "401 :" + arg1;
	else if (replyCode == ERR_NOSUCHSERVER)
		replyMessage += "402 :" + arg1;
	else if (replyCode == ERR_NOSUCHCHANNEL)
		replyMessage += "403 :" + arg1;
	else if (replyCode == ERR_CANNOTSENDTOCHAN)
		replyMessage += "404 :" + arg1;
	else if (replyCode == ERR_TOOMANYCHANNELS)
		replyMessage += "405 :" + arg1;
	else if (replyCode == ERR_WASNOSUCHNICK)
		replyMessage += "406 :" + arg1;
	else if (replyCode == ERR_NORECIPIENT)
		replyMessage += "411 :No recipient given (" + arg1 + ")";
	else if (replyCode == ERR_NOTOPLEVEL)
		replyMessage += "413 :" + arg1;
	else if (replyCode == ERR_WILDTOPLEVEL)
		replyMessage += "414 :" + arg1;
	else if (replyCode == ERR_UNKNOWNCOMMAND)
		replyMessage += "421 :" + arg1;
	else if (replyCode == ERR_NOADMININFO)
		replyMessage += "423 :" + arg1;
	else if (replyCode == ERR_ERRONEUSNICKNAME)
		replyMessage += "432 :" + arg1;
	else if (replyCode == ERR_NICKNAMEINUSE)
		replyMessage += "433 :" + arg1;
	else if (replyCode == ERR_NICKCOLLISION)
		replyMessage += "436 :" + arg1;
	else if (replyCode == ERR_NOTONCHANNEL)
		replyMessage += "442 :" + arg1;
	else if (replyCode == ERR_NOLOGIN)
		replyMessage += "444 :" + arg1;
	else if (replyCode == ERR_NEEDMOREPARAMS)
		replyMessage += "461 :" + arg1;
	else if (replyCode == ERR_KEYSET)
		replyMessage += "467 :" + arg1;
	else if (replyCode == ERR_CHANNELISFULL)
		replyMessage += "471 :" + arg1;
	else if (replyCode == ERR_UNKNOWNMODE)
		replyMessage += "472 :" + arg1;
	else if (replyCode == ERR_INVITEONLYCHAN)
		replyMessage += "473 :" + arg1;
	else if (replyCode == ERR_BANNEDFROMCHAN)
		replyMessage += "474 :" + arg1;
	else if (replyCode == ERR_BADCHANNELKEY)
		replyMessage += "475 :" + arg1;
	else if (replyCode == ERR_BADCHANMASK)
		replyMessage += "476 :" + arg1;
	else if (replyCode == ERR_CHANOPRIVSNEEDED && rplCode--)
		replyMessage += "482 " + _nickname + " :" + arg1 + " :You're not channel operator";

	replyMessage += "\r\n";

	if (rplCode)
		replyMessage.insert(replyMessage.find(':', replyMessage.find(':') + 1), _nickname + " ");

	if (DEBUG)
		std::cout << "\033[1;32mMessage to client: " << _clientSocket << "\n"
				  << replyMessage << "\033[0m";

	send(_clientSocket, replyMessage.c_str(), replyMessage.length(), 0);
}

void Client::reply(ReplyCode replyCode, std::string arg1, std::string arg2)
{
	std::string replyMessage = ":" + _server->getServerHostname() + " ";
	int rplCode = 1;

/* 	if (replyCode == "PRIVMSG" && rplCode--)
		replyMessage = ":" + arg1 + " PRIVMSG " + arg2; */
	if (replyCode == RPL_INVITING)
		replyMessage += "341 :" + arg1 + " " + arg2;
	else if (replyCode == ERR_FILEERROR)
		replyMessage += "424 :File error doing " + arg1 + " on " + arg2;
	else if (replyCode == ERR_USERNOTINCHANNEL)
		replyMessage += "441 :" + arg1 + " " + arg2 + " :They aren't on that channel";
	else if (replyCode == ERR_USERONCHANNEL)
		replyMessage += "443 :" + arg1 + " " + arg2 + " :is already on channel";

	replyMessage += "\r\n";

	if (rplCode)
		replyMessage.insert(replyMessage.find(':', replyMessage.find(':') + 1), _nickname + " ");

	if (DEBUG)
		std::cout << "\033[1;32mMessage to client: " << _clientSocket << "\n"
				  << replyMessage << "\033[0m" << std::endl;

	send(_clientSocket, replyMessage.c_str(), replyMessage.length(), 0);
}

void Client::reply(ReplyCode replyCode, std::string arg1, std::string arg2, std::string arg3)
{
	std::string replyMessage = ":" + _server->getServerHostname() + " ";
	int rplCode = 1;

	if (replyCode == RPL_WHOISUSER)
		replyMessage += "311 :" + _nickname + " " + arg1 + " " + arg2 + arg3; // TODO VERIFIER
	else if (replyCode == RPL_LIST)
		replyMessage += "322 :" + arg1 + " " + arg2 + " :" + arg3;
	else if (replyCode == RPL_TOPICWHOTIME)
		replyMessage += "333 :" + arg1 + " " + arg2 + " " + arg3;
	else if (replyCode == RPL_NAMREPLY)
		replyMessage += "353 :" + arg1 + " " + arg2 + " :" + arg3;

	replyMessage += "\r\n";

	if (rplCode)
		replyMessage.insert(replyMessage.find(':', replyMessage.find(':') + 1), _nickname + " ");

	if (DEBUG)
		std::cout << "\033[1;32mMessage to client: " << _clientSocket << "\n"
				  << replyMessage << "\033[0m" << std::endl;

	send(_clientSocket, replyMessage.c_str(), replyMessage.length(), 0);
}
