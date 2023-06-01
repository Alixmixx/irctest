#include "Client.hpp"
#include "Server.hpp"

void Client::reply(std::string replyMessage) const
{
	replyMessage += "\r\n";
	if (replyMessage.find("PONG") == std::string::npos)
		std::cout << GREEN << "Message to client " << _clientSocket << ":\n"
				  << replyMessage << RESET;
	send(_clientSocket, replyMessage.c_str(), replyMessage.length(), MSG_NOSIGNAL);
}

void Client::reply(std::string replyMessage, ReplyCode replyCode) const
{
	std::stringstream ss;
	ss << ":" << _server->getServerHostname() << " " << std::setfill('0') << std::setw(3)
	   << replyCode << " " << _nickname << " " << replyMessage;
	reply(ss.str());
}

void Client::reply(ReplyCode replyCode, std::string arg1, std::string arg2, std::string arg3,
				   std::string arg4) const
{
	switch (replyCode)
	{
	case RPL_WELCOME:
		return reply(":Welcome to the " + arg1 + " Network, " + arg2 + "[!" + arg3 + "@" + arg4 + "]", replyCode);
	case RPL_YOURHOST:
		return reply(":Your host is " + arg1 + ", running version " + arg2, replyCode);
	case RPL_CREATED:
		return reply(":This server was created on " + arg1, replyCode);
	case RPL_STATSCOMMANDS:
		return reply(arg1 + " " + arg2, replyCode);
	case RPL_STATSCLINE:
		return reply("C " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4, replyCode);
	case RPL_STATSNLINE:
		return reply("N " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4, replyCode);
	case RPL_STATSILINE:
		return reply("I " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4, replyCode);
	case RPL_STATSKLINE:
		return reply("K " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4, replyCode);
	case RPL_STATSYLINE:
		return reply("Y " + arg1 + " " + arg2 + " " + arg3 + " " + arg4, replyCode);
	case RPL_ENDOFSTATS:
		return reply(arg1 + " :End of /STATS report", replyCode);
	case RPL_UMODEIS:
		return reply(arg1, replyCode);
	case RPL_STATSLLINE:
		return reply("L " + arg1 + " * " + arg2 + " " + arg3, replyCode);
	case RPL_STATSUPTIME:
		return reply(":Server Up " + arg1 + " days " + arg2 + ":" + arg3 + ":" + arg4, replyCode);
	case RPL_STATSOLINE:
		return reply("O " + arg1 + " * " + arg2, replyCode);
	case RPL_STATSHLINE:
		return reply("H " + arg1 + " * " + arg2, replyCode);
	case RPL_LUSERCLIENT:
		return reply(":There are " + arg1 + " users and " + arg2 + " invisibles on " + arg3 + " servers", replyCode);
	case RPL_LUSEROP:
		return reply(arg1 + " :IRC operators online", replyCode);
	case RPL_LUSERUNKNOWN:
		return reply(arg1 + " :unknown connections", replyCode);
	case RPL_LUSERCHANNELS:
		return reply(arg1 + " :channels formed", replyCode);
	case RPL_LUSERME:
		return reply(":I have " + arg1 + " clients and " + arg2 + " servers", replyCode);
	case RPL_ADMINME:
		return reply(":" + arg1, replyCode);
	case RPL_ADMINLOC1:
		return reply(":" + arg1, replyCode);
	case RPL_ADMINLOC2:
		return reply(":" + arg1, replyCode);
	case RPL_ADMINEMAIL:
		return reply(":" + arg1, replyCode);
	case RPL_TRACELOG:
		return reply("File " + arg1 + " " + arg2, replyCode);
	case RPL_LOCALUSERS:
		return reply(":Current local users " + arg1 + ", max " + arg2, replyCode);
	case RPL_GLOBALUSERS:
		return reply(":Current global users " + arg1 + ", max " + arg2, replyCode);
	case RPL_NONE:
		return reply("n/a", replyCode);
	case RPL_AWAY:
		return reply(arg1 + " :" + arg2, replyCode);
	case RPL_USERHOST:
		return reply(":" + arg1, replyCode);
	case RPL_UNAWAY:
		return reply(":You are no longer marked as being away", replyCode);
	case RPL_NOWAWAY:
		return reply(":You have been marked as being away", replyCode);
	case RPL_WHOISUSER:
		return reply(arg1 + " " + arg2 + " " + arg3 + " * :" + arg4, replyCode);
	case RPL_WHOISSERVER:
		return reply(arg1 + " " + arg2 + " :" + arg3, replyCode);
	case RPL_WHOISOPERATOR:
		return reply(arg1 + " :is an IRC operator", replyCode);
	case RPL_WHOWASUSER:
		return reply(arg1 + " " + arg2 + " " + arg3 + " * :" + arg4, replyCode);
	case RPL_ENDOFWHO:
		return reply(arg1 + " :End of /WHO list", replyCode);
	case RPL_WHOISIDLE:
		return reply(arg1 + " " + arg2 + " " + arg3 + " :seconds idle, signon time", replyCode);
	case RPL_ENDOFWHOIS:
		return reply(arg1 + " :End of /WHOIS list", replyCode);
	case RPL_WHOISCHANNELS:
		return reply(arg1 + " :" + arg2, replyCode);
	case RPL_LISTSTART:
		return reply("Channel :Users Name", replyCode);
	case RPL_LIST:
		return reply(arg1 + " " + arg2 + " :" + arg3, replyCode);
	case RPL_LISTEND:
		return reply(":End of /LIST", replyCode);
	case RPL_CHANNELMODEIS:
		return reply(arg1 + " " + arg2 + " " + arg3, replyCode);
	case RPL_CREATIONTIME:
		return reply(arg1 + " " + arg2, replyCode);
	case RPL_NOTOPIC:
		return reply(arg1 + " :No topic is set", replyCode);
	case RPL_TOPIC:
		return reply(arg1 + " :" + arg2, replyCode);
	case RPL_TOPICWHOTIME:
		return reply(arg1 + " " + arg2 + " " + arg3, replyCode);
	case RPL_WHOISACTUALLY:
		return reply(arg1 + " " + arg2 + " :actually using host", replyCode);
	case RPL_INVITING:
		return reply(arg1 + " " + arg2, replyCode);
	case RPL_SUMMONING:
		return reply(arg1 + " :Summoning user to IRC", replyCode);
	case RPL_VERSION:
		return reply(arg1 + " " + arg2 + " :" + arg3, replyCode);
	case RPL_WHOREPLY:
		return reply(arg1 + " :0 " + arg2, replyCode);
	case RPL_NAMREPLY:
		return reply(arg1 + " " + arg2 + " :" + arg3, replyCode);
	case RPL_LINKS:
		return reply(arg1 + " " + arg2 + " :" + arg3 + " " + arg4, replyCode);
	case RPL_ENDOFLINKS:
		return reply(arg1 + " :End of /LINKS list", replyCode);
	case RPL_ENDOFNAMES:
		return reply(arg1 + " :End of /NAMES list", replyCode);
	case RPL_BANLIST:
		return reply(arg1 + " " + arg2, replyCode);
	case RPL_ENDOFBANLIST:
		return reply(arg1 + " :End of channel ban list", replyCode);
	case RPL_ENDOFWHOWAS:
		return reply(arg1 + " :End of WHOWAS", replyCode);
	case RPL_INFO:
		return reply(":" + arg1, replyCode);
	case RPL_MOTD:
		return reply(":- " + arg1, replyCode);
	case RPL_ENDOFINFO:
		return reply(":End of /INFO list", replyCode);
	case RPL_MOTDSTART:
		return reply(":- " + arg1 + " Message of the day -", replyCode);
	case RPL_ENDOFMOTD:
		return reply(":End of /MOTD command", replyCode);
	case RPL_YOUREOPER:
		return reply(":You are now an IRC operator", replyCode);
	case RPL_REHASHING:
		return reply(arg1 + " :Rehashing", replyCode);
	case RPL_TIME:
		return reply(arg1 + " :" + arg2, replyCode);
	case RPL_USERSSTART:
		return reply(":UserID Terminal Host", replyCode);
	case RPL_ENDOFUSERS:
		return reply(":End of users", replyCode);
	case RPL_NOUSERS:
		return reply(":Nobody logged in", replyCode);
	case ERR_NOSUCHNICK:
		return reply(arg1 + " :No such nick/channel", replyCode);
	case ERR_NOSUCHSERVER:
		return reply(arg1 + " :No such server", replyCode);
	case ERR_NOSUCHCHANNEL:
		return reply(arg1 + " :No such channel", replyCode);
	case ERR_CANNOTSENDTOCHAN:
		return reply(arg1 + " :Cannot send to channel", replyCode);
	case ERR_TOOMANYCHANNELS:
		return reply(arg1 + " :You have joined too many channels", replyCode);
	case ERR_WASNOSUCHNICK:
		return reply(arg1 + " :There was no such nickname", replyCode);
	case ERR_TOOMANYTARGETS:
		return reply(arg1 + " :Duplicate recipients. No message delivered", replyCode);
	case ERR_NORECIPIENT:
		return reply(":No recipient given (" + arg1 + ")", replyCode);
	case ERR_NOTEXTTOSEND:
		return reply(":No text to send", replyCode);
	case ERR_WILDTOPLEVEL:
		return reply(arg1 + " :Wildcard in toplevel domain", replyCode);
	case ERR_UNKNOWNCOMMAND:
		return reply(arg1 + " :Unknown command", replyCode);
	case ERR_NOMOTD:
		return reply(":MOTD File is missing", replyCode);
	case ERR_NOADMININFO:
		return reply(arg1 + " :No administrative info available", replyCode);
	case ERR_FILEERROR:
		return reply(":File error doing " + arg1 + " on " + arg2, replyCode);
	case ERR_NONICKNAMEGIVEN:
		return reply(":No nickname given", replyCode);
	case ERR_ERRONEUSNICKNAME:
		return reply(arg1 + " :Erroneus nickname", replyCode);
	case ERR_NICKNAMEINUSE:
		return reply(arg1 + " :Nickname is already in use", replyCode);
	case ERR_NICKCOLLISION:
		return reply(arg1 + " :Nickname collision KILL", replyCode);
	case ERR_USERNOTINCHANNEL:
		return reply(arg1 + " " + arg2 + " :They aren't on that channel", replyCode);
	case ERR_NOTONCHANNEL:
		return reply(arg1 + " :You're not on that channel", replyCode);
	case ERR_USERONCHANNEL:
		return reply(arg1 + " " + arg2 + " :is already on channel", replyCode);
	case ERR_NOLOGIN:
		return reply(arg1 + " :User not logged in", replyCode);
	case ERR_SUMMONDISABLED:
		return reply(":SUMMON has been disabled", replyCode);
	case ERR_USERSDISABLED:
		return reply(":USERS has been disabled", replyCode);
	case ERR_NOTREGISTERED:
		return reply(":You have not registered", replyCode);
	case ERR_NEEDMOREPARAMS:
		return reply(arg1 + " :Not enough parameters", replyCode);
	case ERR_ALREADYREGISTRED:
		return reply(":You may not reregister", replyCode);
	case ERR_NOPERMFORHOST:
		return reply(":Your host isn't among the privileged", replyCode);
	case ERR_PASSWDMISMATCH:
		return reply(":Password incorrect", replyCode);
	case ERR_YOUREBANNEDCREEP:
		return reply(":You are banned from this server", replyCode);
	case ERR_KEYSET:
		return reply(arg1 + " :Channel key already set", replyCode);
	case ERR_CHANNELISFULL:
		return reply(arg1 + " :Cannot join channel (+l)", replyCode);
	case ERR_UNKNOWNMODE:
		return reply(arg1 + " :is unknown mode char to me", replyCode);
	case ERR_INVITEONLYCHAN:
		return reply(arg1 + " :Cannot join channel (+i)", replyCode);
	case ERR_BANNEDFROMCHAN:
		return reply(arg1 + " :Cannot join channel (+b)", replyCode);
	case ERR_BADCHANNELKEY:
		return reply(arg1 + " :Cannot join channel (+k)", replyCode);
	case ERR_BADCHANMASK:
		return reply(arg1 + " :Bad Channel Mask", replyCode);
	case ERR_NOPRIVILEGES:
		return reply(":Permission Denied- You're not an IRC operator", replyCode);
	case ERR_CHANOPRIVSNEEDED:
		return reply(arg1 + " :You're not channel operator", replyCode);
	case ERR_CANTKILLSERVER:
		return reply(":You cant kill a server!", replyCode);
	case ERR_NOOPERHOST:
		return reply(arg1 + " :Can only give IRC operator privilege to yourself", replyCode);
	case ERR_UMODEUNKNOWNFLAG:
		return reply(":Unknown MODE flag", replyCode);
	case ERR_USERSDONTMATCH:
		return reply(":Cant change mode for other users", replyCode);
	}
}

void Server::broadcast(std::vector<Client *> recipients, std::string message, Client *except)
{
	for (std::vector<Client *>::const_iterator it = recipients.begin(); it != recipients.end(); ++it)
		if ((*it) != except)
			(*it)->reply(message);
}
