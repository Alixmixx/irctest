#include "Server.hpp"

void Server::replyMessage(Client *client, std::string replyCode)
{
	std::string fullMessage = ":" + _serverHostname + " ";
	std::string replyMessage;

	if (replyCode == "RPL_WELCOME")
		replyMessage = "001 :Welcome to the <networkname> Network, " + client->getNickname() + "[!" + client->getUsername() + "@" + client->getHostname() + "]";
	else if (replyCode == "RPL_YOURHOST")
		replyMessage = "002 :Your host is " + _serverName + ", running version " + _serverVersion;
	else if (replyCode == "RPL_CREATED")
		replyMessage = "003 :This server was created " + _serverCreationDate + " at " + _serverCreationTime;
	else if (replyCode == "RPL_MYINFO")
		replyMessage = "004 :" + _serverName + " " + _serverVersion + " <available user modes> <available channel modes>";
	else if (replyCode == "RPL_ISUPPORT")
		replyMessage = "005 :"; //"005 :<1-13 tokens>  :are supported by this server";
	else if (replyCode == "RPL_UMODEIS")
		replyMessage = "221 :<user modes>"; // Client.getMode(); <<-- creer une fonction getMode dans client apres avoir lu la doc
	else if (replyCode == "RPL_STATSUPTIME")
		replyMessage = "242 :Server Up <days> days <hours>:<minutes>:<seconds>"; // + getServerUptime(); <<-- creer une fonction getServerUptime qui retourne le temps depuis le lancement du serv
	else if (replyCode == "ERR_NOMOTD")
		replyMessage = "422 :MOTD File is missing";
	else if (replyCode == "ERR_NOORIGIN")
		replyMessage = "409 :No origin specified";
	else if (replyCode == "ERR_NOTEXTTOSEND")
		replyMessage = "412 :No text to send";
	else if (replyCode == "ERR_NONICKNAMEGIVEN")
		replyMessage = "431 :No nickname given";
	else if (replyCode == "ERR_SUMMONDISABLED")
		replyMessage = "445 :SUMMON has been disabled";
	else if (replyCode == "ERR_USERSDISABLED")
		replyMessage = "446 :USERS has been disabled";
	else if (replyCode == "ERR_NOTREGISTERED")
		replyMessage = "451 :You have not registered";
	else if (replyCode == "ERR_ALREADYREGISTRED")
		replyMessage = "462 :You may not reregister";
	else if (replyCode == "ERR_NOPERMFORHOST")
		replyMessage = "463 :Your host isn't among the privileged";
	else if (replyCode == "ERR_PASSWDMISMATCH")
		replyMessage = "464 :Password incorrect";
	else if (replyCode == "ERR_YOUREBANNEDCREEP")
		replyMessage = "465 :You are banned from this server";
	else if (replyCode == "ERR_NOPRIVILEGES")
		replyMessage = "481 :Permission Denied- You're not an IRC operator";
	else if (replyCode == "ERR_CANTKILLSERVER")
		replyMessage = "483 :You cant kill a server!";
	else if (replyCode == "ERR_NOOPERHOST")
		replyMessage = "491 :No O-lines for your host";
	else if (replyCode == "ERR_UMODEUNKNOWNFLAG")
		replyMessage = "501 :Unknown MODE flag";
	else if (replyCode == "ERR_USERSDONTMATCH")
		replyMessage = "502 :Cant change mode for other users";

	fullMessage += replyMessage.insert(3, " " + client->getNickname()) + "\r\n";

	if (DEBUG)
		std::cout << "\n\n"
				  << fullMessage << std::endl;

	send(client->getSocket(), fullMessage.c_str(), fullMessage.length(), 0);
}

void Server::replyMessage(Client *client, std::string replyCode, std::string arg1)
{
	std::string fullMessage = ":" + _serverHostname + " ";
	std::string replyMessage = "";

	if (replyCode == "ERR_NOSUCHNICK")
		replyMessage = "401 :" + arg1;
	else if (replyCode == "ERR_NOSUCHSERVER")
		replyMessage = "402 :" + arg1;
	else if (replyCode == "ERR_NOSUCHCHANNEL")
		replyMessage = "403 :" + arg1;
	else if (replyCode == "ERR_CANNOTSENDTOCHAN")
		replyMessage = "404 :" + arg1;
	else if (replyCode == "ERR_TOOMANYCHANNELS")
		replyMessage = "405 :" + arg1;
	else if (replyCode == "ERR_WASNOSUCHNICK")
		replyMessage = "406 :" + arg1;
	else if (replyCode == "ERR_NORECIPIENT")
		replyMessage = "411 :No recipient given (" + arg1 + ")";
	else if (replyCode == "ERR_NOTOPLEVEL")
		replyMessage = "413 :" + arg1 + " :No toplevel domain specified";
	else if (replyCode == "ERR_WILDTOPLEVEL")
		replyMessage = "414 :" + arg1 + " :Wildcard in toplevel domain";
	else if (replyCode == "ERR_UNKNOWNCOMMAND")
		replyMessage = "421 :" + arg1 + " :Unknown command";
	else if (replyCode == "ERR_NOADMININFO")
		replyMessage = "423 :" + arg1 + " :No administrative info available";
	else if (replyCode == "ERR_ERRONEUSNICKNAME")
		replyMessage = "432 :" + arg1 + " :Erroneus nickname";
	else if (replyCode == "ERR_NICKNAMEINUSE")
		replyMessage = "433 :" + arg1;
	else if (replyCode == "ERR_NICKCOLLISION")
		replyMessage = "436 :" + arg1 + " :Nickname collision KILL";
	else if (replyCode == "ERR_NOTONCHANNEL")
		replyMessage = "442 :" + arg1 + " :You're not on that channel";
	else if (replyCode == "ERR_NOLOGIN")
		replyMessage = "444 :" + arg1 + " :User not logged in";
	else if (replyCode == "ERR_NEEDMOREPARAMS")
		replyMessage = "461 :" + arg1 + " :Not enough parameters";
	else if (replyCode == "ERR_KEYSET")
		replyMessage = "467 :" + arg1 + " :Channel key already set";
	else if (replyCode == "ERR_CHANNELISFULL")
		replyMessage = "471 :" + arg1 + " :Cannot join channel (+l)";
	else if (replyCode == "ERR_UNKNOWNMODE")
		replyMessage = "472 :" + arg1 + " :is unknown mode char to me";
	else if (replyCode == "ERR_INVITEONLYCHAN")
		replyMessage = "473 :" + arg1 + " :Cannot join channel (+i)";
	else if (replyCode == "ERR_BANNEDFROMCHAN")
		replyMessage = "474 :" + arg1 + " :Cannot join channel (+b)";
	else if (replyCode == "ERR_BADCHANNELKEY")
		replyMessage = "475 :" + arg1 + " :Cannot join channel (+k)";
	else if (replyCode == "ERR_CHANOPRIVSNEEDED")
		replyMessage = "482 :" + arg1 + " :You're not channel operator";

	if (replyMessage.empty())
		fullMessage += replyCode + " " + arg1 + "\r\n";
	else
		fullMessage += replyMessage.insert(3, " " + client->getNickname()) + "\r\n";

	if (DEBUG)
		std::cout << "\n\n"
				  << fullMessage << std::endl;

	send(client->getSocket(), fullMessage.c_str(), fullMessage.length(), 0);
}

void Server::replyMessage(Client *client, std::string replyCode, std::string arg1, std::string arg2)
{
	std::string replyMessage = ":" + _serverHostname + " ";
	int rplCode = 1;

	if (replyCode == "PRIVMSG" && rplCode--)
		replyMessage = ":" + arg1 + " PRIVMSG " + arg2;
	if (replyCode == "RPL_NICKCHANGE" && rplCode--)
		replyMessage = ":" + arg1 + " NICK :" + arg2;
	else if (replyCode == "ERR_FILEERROR")
		replyMessage += "424 :File error doing " + arg1 + " on " + arg2;
	else if (replyCode == "ERR_USERNOTINCHANNEL")
		replyMessage += "441 :" + arg1 + " " + arg2 + " :They aren't on that channel";
	else if (replyCode == "ERR_USERONCHANNEL")
		replyMessage += "443 :" + arg1 + " " + arg2 + " :is already on channel";

	if (rplCode)
		replyMessage = replyMessage.insert(3, " " + client->getNickname()) + "\r\n";
	else
		replyMessage += "\r\n";

	if (DEBUG)
		std::cout << "\n\n"<< replyMessage << std::endl;

	send(client->getSocket(), replyMessage.c_str(), replyMessage.length(), 0);
}

void Server::replyMessage(Client *client, std::string replyCode, std::string arg1, std::string arg2, std::string arg3)
{
	std::string fullMessage = ":" + _serverHostname + " ";
	std::string replyMessage = "";

	if (replyCode == "RPL_WHOISUSER")
		replyMessage = "311 :" + client->getNickname() + " " + arg1 + " " + arg2 + arg3;

	if (replyMessage.empty())
		fullMessage += replyCode + " " + arg1 + " " + arg2 + " " + arg3 + "\r\n";
	else
		fullMessage += replyMessage.insert(3, " " + client->getNickname()) + "\r\n";

	if (DEBUG)
		std::cout << "\n\n"
				  << fullMessage << std::endl;

	send(client->getSocket(), fullMessage.c_str(), fullMessage.length(), 0);
}