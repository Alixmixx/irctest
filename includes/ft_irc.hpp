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
	ERR_ALREADYREGISTERED,
	ERR_BADCHANMASK,
	ERR_BADCHANNELKEY,
	ERR_BANNEDFROMCHAN,
	ERR_CANNOTSENDTOCHAN,
	ERR_CANTKILLSERVER,
	ERR_CHANNELISFULL,
	ERR_CHANOPRIVSNEEDED,
	ERR_ERRONEUSNICKNAME,
	ERR_FILEERROR,
	ERR_INVITEONLYCHAN,
	ERR_KEYSET,
	ERR_NEEDMOREPARAMS,
	ERR_NICKCOLLISION,
	ERR_NICKNAMEINUSE,
	ERR_NOADMININFO,
	ERR_NOLOGIN,
	ERR_NOMOTD,
	ERR_NONICKNAMEGIVEN,
	ERR_NOOPERHOST,
	ERR_NOORIGIN,
	ERR_NOPERMFORHOST,
	ERR_NOPRIVILEGES,
	ERR_NORECIPIENT,
	ERR_NOSUCHCHANNEL,
	ERR_NOSUCHNICK,
	ERR_NOSUCHSERVER,
	ERR_NOTEXTTOSEND,
	ERR_NOTONCHANNEL,
	ERR_NOTOPLEVEL,
	ERR_NOTREGISTERED,
	ERR_PASSWDMISMATCH,
	ERR_SUMMONDISABLED,
	ERR_TOOMANYCHANNELS,
	ERR_UMODEUNKNOWNFLAG,
	ERR_UNKNOWNCOMMAND,
	ERR_UNKNOWNMODE,
	ERR_USERNOTINCHANNEL,
	ERR_USERONCHANNEL,
	ERR_USERSDISABLED,
	ERR_USERSDONTMATCH,
	ERR_WASNOSUCHNICK,
	ERR_WILDTOPLEVEL,
	ERR_YOUREBANNEDCREEP,
	RPL_CREATED,
	RPL_ENDOFNAMES,
	RPL_ENDOFWHOIS,
	RPL_INVITING,
	RPL_ISUPPORT,
	RPL_LIST,
	RPL_LISTEND,
	RPL_LISTSTART,
	RPL_MYINFO,
	RPL_NAMREPLY,
	RPL_NOTOPIC,
	RPL_STATSUPTIME,
	RPL_TOPICWHOTIME,
	RPL_UMODEIS,
	RPL_WELCOME,
	RPL_WHOISUSER,
	RPL_YOURHOST
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
std::string getCurrentDateTime();
std::string toString(int number);


