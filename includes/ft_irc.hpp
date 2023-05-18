#pragma once

#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

typedef enum ReplyCode
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
}	ReplyCode;

typedef enum Modes
{
	BANNED,
	NOTINCHANNEL,
	INVITED,
	USER,
	MODERATOR,
	OPERATOR,
	PROTECTED,
	FOUNDER
}	Modes;

#define SERVERNAME "MiaoRC"
#define SERVERHOSTNAME "irc.125.outstanding.gov"
#define SERVERVERSION "0.042a"
#define INFO "42School"
#define BACKLOG 128 // why 128 ?
#define MAX_CLIENTS 1024
#define MAX_EVENTS 32 // why 10 ?
#define BUFFER_SIZE 256
#define MAX_CHANNELS_PER_CLIENT 10
#define MAX_USERS_PER_CHANNEL 10
#define DEBUG true

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

bool isPortNumberCorrect(std::string port);
bool isPasswordCorrect(std::string password);
std::string concatenateArguments(std::vector<std::string> arguments, unsigned int start);
std::vector<std::string> split(const std::string &str, char delim);
std::string toString(long number);
