#ifndef REPLYCODE_HPP
#define REPLYCODE_HPP

enum ReplyCode
{
	RPL_WELCOME,
	RPL_YOURHOST,
	RPL_CREATED,
	RPL_MYINFO,
	RPL_ISUPPORT,
	RPL_UMODEIS,
	RPL_STATSUPTIME,
	RPL_LUSERCLIENT,
	RPL_LUSEROP,
	RPL_LUSERUNKNOWN,
	RPL_LUSERCHANNELS,
	RPL_LUSERME,
	RPL_ADMINME,
	RPL_ADMINLOC1,
	RPL_ADMINLOC2,
	RPL_ADMINEMAIL,
	RPL_TRACEEND,
	RPL_AWAY,
	RPL_USERHOST,
	RPL_ISON,
	RPL_UNAWAY,
	RPL_NOWAWAY,
	RPL_WHOISOPERATOR,
	RPL_WHOWASUSER,
	RPL_ENDOFWHO,
	RPL_WHOISIDLE,
	RPL_ENDOFWHOIS,
	RPL_WHOISCHANNELS,
	RPL_LIST,
	RPL_LISTEND,
	RPL_CHANNELMODEIS,
	RPL_CREATIONTIME,
	ERR_NOTOPIC,
	RPL_TOPIC,
	RPL_INVITING,
	RPL_VERSION,
	RPL_WHOREPLY,
	RPL_NAMREPLY,
	RPL_LINKS,
	RPL_ENDOFLINKS,
	RPL_ENDOFNAMES,
	RPL_BANLIST,
	RPL_ENDOFBANLIST,
	RPL_ENDOFWHOWAS,
	RPL_INFO,
	RPL_MOTD,
	RPL_ENDOFINFO,
	RPL_MOTDSTART,
	RPL_ENDOFMOTD,
	RPL_YOUREOPER,
	RPL_REHASHING,
	RPL_TIME,
	ERR_NOSUCHNICK,
	ERR_NOSUCHSERVER,
	ERR_NOSUCHCHANNEL,
	ERR_CANNOTSENDTOCHAN,
	ERR_WASNOSUCHNICK,
	ERR_NOORIGIN,
	ERR_INVALIDCAP,
	ERR_NORECIPIENT,
	ERR_NOTEXTTOSEND,
	ERR_UNKNOWNCOMMAND,
	ERR_NOMOTD,
	ERR_NOADMININFO,
	ERR_FILEERROR,
	ERR_NONICKNAMEGIVEN,
	ERR_ERRONEUSNICKNAME,
	ERR_NICKNAMEINUSE,
	ERR_NICKCOLLISION,
	ERR_USERNOTINCHANNEL,
	ERR_NOTONCHANNEL,
	ERR_USERONCHANNEL,
	ERR_NOTREGISTERED,
	ERR_NEEDMOREPARAMS,
	ERR_ALREADYREGISTRED,
	ERR_CHANNELISFULL,
	ERR_UNKNOWNMODE,
	ERR_INVITEONLYCHAN,
	ERR_BANNEDFROMCHAN,
	ERR_BADCHANNELKEY,
	ERR_BADCHANMASK,
	ERR_NOPRIVILEGES,
	ERR_CHANOPRIVSNEEDED,
	ERR_CANTKILLSERVER,
	ERR_NOOPERHOST,
	ERR_UMODEUNKNOWNFLAG,
	ERR_USERSDONTMATCH,
	ERR_ALREADYBAN,
	ERR_KILLDENY
};


#endif