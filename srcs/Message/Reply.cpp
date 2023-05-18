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

void Client::addInfo(int numeric, std::string replyMessage) const
{
	std::stringstream ss;
	ss << ":" << _server->getServerHostname() << " " << std::setfill('0') << std::setw(3) << numeric << " " << _nickname << " " << replyMessage;
	reply(ss.str());
}

void Client::reply(ReplyCode replyCode, std::string arg1, std::string arg2, std::string arg3, std::string arg4) const
{
	switch (replyCode)
		{
		case RPL_WELCOME:
			return addInfo(1, ":Welcome to the " + arg1 + " Network, " + arg2 + "[!" + arg3 + "@" + arg4 + "]");
		case RPL_YOURHOST:
			return addInfo(2, ":Your host is " + arg1 + ", running version " + arg2);
		case RPL_CREATED:
			return addInfo(3, ":This server was created on " + arg1);
		case RPL_STATSCOMMANDS:
			return addInfo(212, arg1 + " " + arg2);
		case RPL_STATSCLINE:
			return addInfo(213, "C " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4);
		case RPL_STATSNLINE:
			return addInfo(214, "N " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4);
		case RPL_STATSILINE:
			return addInfo(215, "I " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4);
		case RPL_STATSKLINE:
			return addInfo(216, "K " + arg1 + " * " + arg2 + " " + arg3 + " " + arg4);
		case RPL_STATSYLINE:
			return addInfo(218, "Y " + arg1 + " " + arg2 + " " + arg3 + " " + arg4);
		case RPL_ENDOFSTATS:
			return addInfo(219, arg1 + " :End of /STATS report");
		case RPL_UMODEIS:
			return addInfo(221, arg1);
		case RPL_STATSLLINE:
			return addInfo(241, "L " + arg1 + " * " + arg2 + " " + arg3);
		case RPL_STATSUPTIME:
			return addInfo(242, ":Server Up " + arg1 + " days " + arg2 + ":" + arg3 + ":" + arg4);
		case RPL_STATSOLINE:
			return addInfo(243, "O " + arg1 + " * " + arg2);
		case RPL_STATSHLINE:
			return addInfo(244, "H " + arg1 + " * " + arg2);
		case RPL_LUSERCLIENT:
			return addInfo(251, ":There are " + arg1 + " users and " + arg2 + " invisible on " + arg3 + " servers");
		case RPL_LUSEROP:
			return addInfo(252, arg1 + " :operator(s) online");
		case RPL_LUSERUNKNOWN:
			return addInfo(253, arg1 + " :unknown connection(s)");
		case RPL_LUSERCHANNELS:
			return addInfo(254, arg1 + " :channels formed");
		case RPL_LUSERME:
			return addInfo(255, ":I have " + arg1 + " clients and " + arg2 + " servers");
		case RPL_ADMINME:
			return addInfo(256, arg1 + " :Administrative info");
		case RPL_ADMINLOC1:
			return addInfo(257, ":" + arg1);
		case RPL_ADMINLOC2:
			return addInfo(258, ":" + arg1);
		case RPL_ADMINEMAIL:
			return addInfo(259, ":" + arg1);
		case RPL_TRACELOG:
			return addInfo(261, "File " + arg1 + " " + arg2);
		case RPL_NONE:
			return addInfo(300, "n/a");
		case RPL_AWAY:
			return addInfo(301, arg1 + " :" + arg2);
		case RPL_UNAWAY:
			return addInfo(305, ":You are no longer marked as being away");
		case RPL_NOWAWAY:
			return addInfo(306, ":You have been marked as being away");
		case RPL_WHOISUSER:
			return addInfo(311, arg1 + " " + arg2 + " " + arg3 + " * :" + arg4);
		case RPL_WHOISSERVER:
			return addInfo(312, arg1 + " " + arg2 + " :" + arg3);
		case RPL_WHOISOPERATOR:
			return addInfo(313, arg1 + " :is an IRC operator");
		case RPL_WHOWASUSER:
			return addInfo(314, arg1 + " " + arg2 + " " + arg3 + " * :" + arg4);
		case RPL_ENDOFWHO:
			return addInfo(315, arg1 + " :End of /WHO list");
		case RPL_WHOISIDLE:
			return addInfo(317, arg1 + " " + arg2 + " :seconds idle");
		case RPL_ENDOFWHOIS:
			return addInfo(318, arg1 + " :End of /WHOIS list");
		case RPL_LISTSTART:
			return addInfo(321, "Channel :Users Name");
		case RPL_LIST:
			return addInfo(322, arg1 + " " + arg2 + " :" + arg3);
		case RPL_LISTEND:
			return addInfo(323, ":End of /LIST");
		case RPL_CHANNELMODEIS:
			return addInfo(324, arg1 + " " + arg2 + " " + arg3);
		case RPL_NOTOPIC:
			return addInfo(331, arg1 + " :No topic is set");
		case RPL_TOPIC:
			return addInfo(332, arg1 + " :" + arg2);
		case RPL_INVITING:
			return addInfo(341, arg1 + " " + arg2);
		case RPL_SUMMONING:
			return addInfo(342, arg1 + " :Summoning user to IRC");
		case RPL_VERSION:
			return addInfo(351, arg1 + " " + arg2 + " :" + arg3);
		case RPL_LINKS:
			return addInfo(364, arg1 + " " + arg2 + " :" + arg3 + " " + arg4);
		case RPL_ENDOFLINKS:
			return addInfo(365, arg1 + " :End of /LINKS list");
		case RPL_ENDOFNAMES:
			return addInfo(366, arg1 + " :End of /NAMES list");
		case RPL_BANLIST:
			return addInfo(367, arg1 + " " + arg2);
		case RPL_ENDOFBANLIST:
			return addInfo(368, arg1 + " :End of channel ban list");
		case RPL_ENDOFWHOWAS:
			return addInfo(369, arg1 + " :End of WHOWAS");
		case RPL_INFO:
			return addInfo(371, ":" + arg1);
		case RPL_MOTD:
			return addInfo(372, ":- " + arg1);
		case RPL_ENDOFINFO:
			return addInfo(374, ":End of /INFO list");
		case RPL_MOTDSTART:
			return addInfo(375, ":- " + arg1 + " Message of the day -");
		case RPL_ENDOFMOTD:
			return addInfo(376, ":End of /MOTD command");
		case RPL_YOUREOPER:
			return addInfo(381, ":You are now an IRC operator");
		case RPL_REHASHING:
			return addInfo(382, arg1 + " :Rehashing");
		case RPL_TIME:
			return addInfo(391, arg1 + " :" + arg2);
		case RPL_USERSSTART:
			return addInfo(392, ":UserID Terminal Host");
		case RPL_ENDOFUSERS:
			return addInfo(394, ":End of users");
		case RPL_NOUSERS:
			return addInfo(395, ":Nobody logged in");
		case ERR_NOSUCHNICK:
			return addInfo(401, arg1 + " :No such nick/channel");
		case ERR_NOSUCHSERVER:
			return addInfo(402, arg1 + " :No such server");
		case ERR_NOSUCHCHANNEL:
			return addInfo(403, arg1 + " :No such channel");
		case ERR_CANNOTSENDTOCHAN:
			return addInfo(404, arg1 + " :Cannot send to channel");
		case ERR_TOOMANYCHANNELS:
			return addInfo(405, arg1 + " :You have joined too many channels");
		case ERR_WASNOSUCHNICK:
			return addInfo(406, arg1 + " :There was no such nickname");
		case ERR_TOOMANYTARGETS:
			return addInfo(407, arg1 + " :Duplicate recipients. No message delivered");
		case ERR_NOORIGIN:
			return addInfo(409, ":No origin specified");
		case ERR_NORECIPIENT:
			return addInfo(411, ":No recipient given (" + arg1 + ")");
		case ERR_NOTEXTTOSEND:
			return addInfo(412, ":No text to send");
		case ERR_NOTTOPLEVEL:
			return addInfo(413, arg1 + " :No toplevel domain specified");
		case ERR_WILDTOPLEVEL:
			return addInfo(414, arg1 + " :Wildcard in toplevel domain");
		case ERR_UNKNOWNCOMMAND:
			return addInfo(421, arg1 + " :Unknown command");
		case ERR_NOMOTD:
			return addInfo(422, ":MOTD File is missing");
		case ERR_NOADMININFO:
			return addInfo(423, arg1 + " :No administrative info available");
		case ERR_FILEERROR:
			return addInfo(424, ":File error doing " + arg1 + " on " + arg2);
		case ERR_NONICKNAMEGIVEN:
			return addInfo(431, ":No nickname given");
		case ERR_ERRONEUSNICKNAME:
			return addInfo(432, arg1 + " :Erroneus nickname");
		case ERR_NICKNAMEINUSE:
			return addInfo(433, arg1 + " :Nickname is already in use");
		case ERR_NICKCOLLISION:
			return addInfo(436, arg1 + " :Nickname collision KILL");
		case ERR_USERNOTINCHANNEL:
			return addInfo(441, arg1 + " " + arg2 + " :They aren't on that channel");
		case ERR_NOTONCHANNEL:
			return addInfo(442, arg1 + " :You're not on that channel");
		case ERR_USERONCHANNEL:
			return addInfo(443, arg1 + " " + arg2 + " :is already on channel");
		case ERR_NOLOGIN:
			return addInfo(444, arg1 + " :User not logged in");
		case ERR_SUMMONDISABLED:
			return addInfo(445, ":SUMMON has been disabled");
		case ERR_USERSDISABLED:
			return addInfo(446, ":USERS has been disabled");
		case ERR_NOTREGISTERED:
			return addInfo(451, ":You have not registered");
		case ERR_NEEDMOREPARAMS:
			return addInfo(461, arg1 + " :Not enough parameters");
		case ERR_ALREADYREGISTERED:
			return addInfo(462, ":You may not reregister");
		case ERR_NOPERMFORHOST:
			return addInfo(463, ":Your host isn't among the privileged");
		case ERR_PASSWDMISMATCH:
			return addInfo(464, ":Password incorrect");
		case ERR_YOUREBANNEDCREEP:
			return addInfo(465, ":You are banned from this server");
		case ERR_KEYSET:
			return addInfo(467, arg1 + " :Channel key already set");
		case ERR_CHANNELISFULL:
			return addInfo(471, arg1 + " :Cannot join channel (+l)");
		case ERR_UNKNOWNMODE:
			return addInfo(472, arg1 + " :is unknown mode char to me");
		case ERR_INVITEONLYCHAN:
			return addInfo(473, arg1 + " :Cannot join channel (+i)");
		case ERR_BANNEDFROMCHAN:
			return addInfo(474, arg1 + " :Cannot join channel (+b)");
		case ERR_BADCHANNELKEY:
			return addInfo(475, arg1 + " :Cannot join channel (+k)");
		case ERR_NOPRIVILEGES:
			return addInfo(481, ":Permission Denied- You're not an IRC operator");
		case ERR_CHANOPRIVSNEEDED:
			return addInfo(482, arg1 + " :You're not channel operator");
		case ERR_CANTKILLSERVER:
			return addInfo(483, ":You cant kill a server!");
		case ERR_NOOPERHOST:
			return addInfo(491, ":No O-lines for your host");
		case ERR_UMODEUNKNOWNFLAG:
			return addInfo(501, ":Unknown MODE flag");
		case ERR_USERSDONTMATCH:
			return addInfo(502, ":Cant change mode for other users");
		default:
			exit(OUTSTANDING_ERROR);
		}
}
