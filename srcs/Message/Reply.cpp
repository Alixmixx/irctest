#include "Client.hpp"

void Client::reply(std::string replyMessage) const
{
	replyMessage += "\r\n";

	if (DEBUG)
		std::cout << "\033[1;32mMessage to client: " << _clientSocket << "\n"
				  << replyMessage << "\033[0m" << std::endl;

	send(_clientSocket, replyMessage.c_str(), replyMessage.length(), 0);
}

std::string Client::prefix() const
{
	return ":" + _nickname + "!" + _username + "@" + _hostname;
}

void Client::addInfo(std::string replyMessage) const
{
	reply(":" + _server->getServerHostname() + " " + replyMessage.substr(0, 4) + _nickname + " " + replyMessage.substr(4));
}

void Client::reply(ReplyCode replyCode, std::string arg1, std::string arg2, std::string arg3) const
{
	switch (replyCode)
	{
	case RPL_WELCOME:
		return addInfo("001 :Welcome to the " + std::string(NETWORKNAME) + " " + _nickname + "[!" + _username + "@" + _hostname + "]");
	case RPL_YOURHOST:
		return addInfo("002 :Your host is " + _server->getServerName() + ", running version " + _server->getServerVersion());
	case RPL_CREATED:
		return addInfo("003 :This server was created on " + formatTime(_server->getServerCreationTime()));
	case RPL_MYINFO:
		return addInfo("004 :" + _server->getServerName() + " " + _server->getServerVersion() + " <available user modes> <available channel modes>");
	case RPL_ISUPPORT:
		return addInfo("005 :"); //"005 :<1-13 tokens>  :are supported by this server");
	case RPL_UMODEIS:
		return addInfo("221 :<user modes>"); // Client.getMode()); <<-- creer une fonction getMode dans client apres avoir lu la doc
	case RPL_STATSUPTIME:
		return addInfo("242 :Server Up <days> days <hours>:<minutes>:<seconds>"); // + getServerUptime()); <<-- creer une fonction getServerUptime qui retourne le temps depuis le lancement du serv
	case RPL_LISTSTART:
		return addInfo("321 :Channel :Users Name");
	case RPL_LISTEND:
		return addInfo("323 :End of /LIST");
	case ERR_NOMOTD:
		return addInfo("422 :MOTD File is missing");
	case ERR_NOORIGIN:
		return addInfo("409 :No origin specified");
	case ERR_NOTEXTTOSEND:
		return addInfo("412 :No text to send");
	case ERR_NONICKNAMEGIVEN:
		return addInfo("431 :No nickname given");
	case ERR_SUMMONDISABLED:
		return addInfo("445 :SUMMON has been disabled");
	case ERR_USERSDISABLED:
		return addInfo("446 :USERS has been disabled");
	case ERR_NOTREGISTERED:
		return addInfo("451 :You have not registered");
	case ERR_ALREADYREGISTERED:
		return addInfo("462 :You may not reregister");
	case ERR_NOPERMFORHOST:
		return addInfo("463 :Your host isn't among the privileged");
	case ERR_PASSWDMISMATCH:
		return addInfo("464 :Password incorrect");
	case ERR_YOUREBANNEDCREEP:
		return addInfo("465 :You are banned from this server");
	case ERR_NOPRIVILEGES:
		return addInfo("481 :Permission Denied- You're not an IRC operator");
	case ERR_CANTKILLSERVER:
		return addInfo("483 :You cant kill a server!");
	case ERR_NOOPERHOST:
		return addInfo("491 :No O-lines for your host");
	case ERR_UMODEUNKNOWNFLAG:
		return addInfo("501 :Unknown MODE flag");
	case ERR_USERSDONTMATCH:
		return addInfo("502 :Cant change mode for other users");
	case RPL_ENDOFWHOIS:
		return addInfo("318 :" + arg1);
	case RPL_NOTOPIC:
		return addInfo("331 :" + arg1);
	case RPL_ENDOFNAMES:
		return addInfo("366 :" + arg1);
	case ERR_NOSUCHNICK:
		return addInfo("401 :" + arg1);
	case ERR_NOSUCHSERVER:
		return addInfo("402 :" + arg1);
	case ERR_NOSUCHCHANNEL:
		return addInfo("403 :" + arg1);
	case ERR_CANNOTSENDTOCHAN:
		return addInfo("404 :" + arg1);
	case ERR_TOOMANYCHANNELS:
		return addInfo("405 :" + arg1);
	case ERR_WASNOSUCHNICK:
		return addInfo("406 :" + arg1);
	case ERR_NORECIPIENT:
		return addInfo("411 :No recipient given (" + arg1 + ")");
	case ERR_NOTOPLEVEL:
		return addInfo("413 :" + arg1);
	case ERR_WILDTOPLEVEL:
		return addInfo("414 :" + arg1);
	case ERR_UNKNOWNCOMMAND:
		return addInfo("421 :" + arg1);
	case ERR_NOADMININFO:
		return addInfo("423 :" + arg1);
	case ERR_ERRONEUSNICKNAME:
		return addInfo("432 :" + arg1);
	case ERR_NICKNAMEINUSE:
		return addInfo("433 :" + arg1);
	case ERR_NICKCOLLISION:
		return addInfo("436 :" + arg1);
	case ERR_NOTONCHANNEL:
		return addInfo("442 :" + arg1);
	case ERR_NOLOGIN:
		return addInfo("444 :" + arg1);
	case ERR_NEEDMOREPARAMS:
		return addInfo("461 :" + arg1);
	case ERR_KEYSET:
		return addInfo("467 :" + arg1);
	case ERR_CHANNELISFULL:
		return addInfo("471 :" + arg1);
	case ERR_UNKNOWNMODE:
		return addInfo("472 :" + arg1);
	case ERR_INVITEONLYCHAN:
		return addInfo("473 :" + arg1);
	case ERR_BANNEDFROMCHAN:
		return addInfo("474 :" + arg1);
	case ERR_BADCHANNELKEY:
		return addInfo("475 :" + arg1);
	case ERR_BADCHANMASK:
		return addInfo("476 :" + arg1);
	case ERR_CHANOPRIVSNEEDED:
		return addInfo("482 :" + arg1 + " :You're not channel operator");
	case RPL_INVITING:
		return addInfo("341 :" + arg1 + " " + arg2);
	case ERR_FILEERROR:
		return addInfo("424 :File error doing " + arg1 + " on " + arg2);
	case ERR_USERNOTINCHANNEL:
		return addInfo("441 :" + arg1 + " " + arg2 + " :They aren't on that channel");
	case ERR_USERONCHANNEL:
		return addInfo("443 :" + arg1 + " " + arg2 + " :is already on channel");
	case RPL_WHOISUSER:
		return addInfo("311 :" + _nickname + " " + arg1 + " " + arg2 + arg3); // TODO VERIFIER
	case RPL_LIST:
		return addInfo("322 :" + arg1 + " " + arg2 + " :" + arg3);
	case RPL_TOPICWHOTIME:
		return addInfo("333 :" + arg1 + " " + arg2 + " " + arg3);
	case RPL_NAMREPLY:
		return addInfo("353 :" + arg1 + " " + arg2 + " :" + arg3);
	default:
		exit(OUTSTANDING_ERROR);
	}
}