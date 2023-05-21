#pragma once

#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

typedef enum ReplyCode {
	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED = 3,
	RPL_STATSCOMMANDS = 212,
	RPL_STATSCLINE = 213,
	RPL_STATSNLINE = 214,
	RPL_STATSILINE = 215,
	RPL_STATSKLINE = 216,
	RPL_STATSYLINE = 218,
	RPL_ENDOFSTATS = 219,
	RPL_UMODEIS = 221,
	RPL_STATSLLINE = 241,
	RPL_STATSUPTIME = 242,
	RPL_STATSOLINE = 243,
	RPL_STATSHLINE = 244,
	RPL_LUSERCLIENT = 251,
	RPL_LUSEROP = 252,
	RPL_LUSERUNKNOWN = 253,
	RPL_LUSERCHANNELS = 254,
	RPL_LUSERME = 255,
	RPL_ADMINME = 256,
	RPL_ADMINLOC1 = 257,
	RPL_ADMINLOC2 = 258,
	RPL_ADMINEMAIL = 259,
	RPL_TRACELOG = 261,
	RPL_NONE = 300,
	RPL_AWAY = 301,
	RPL_UNAWAY = 305,
	RPL_NOWAWAY = 306,
	RPL_WHOISUSER = 311,
	RPL_WHOISSERVER = 312,
	RPL_WHOISOPERATOR = 313,
	RPL_WHOWASUSER = 314,
	RPL_ENDOFWHO = 315,
	RPL_WHOISIDLE = 317,
	RPL_ENDOFWHOIS = 318,
	RPL_LISTSTART = 321,
	RPL_LIST = 322,
	RPL_LISTEND = 323,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_INVITING = 341,
	RPL_SUMMONING = 342,
	RPL_VERSION = 351,
	RPL_LINKS = 364,
	RPL_ENDOFLINKS = 365,
	RPL_ENDOFNAMES = 366,
	RPL_BANLIST = 367,
	RPL_ENDOFBANLIST = 368,
	RPL_ENDOFWHOWAS = 369,
	RPL_INFO = 371,
	RPL_MOTD = 372,
	RPL_ENDOFINFO = 374,
	RPL_MOTDSTART = 375,
	RPL_ENDOFMOTD = 376,
	RPL_YOUREOPER = 381,
	RPL_REHASHING = 382,
	RPL_TIME = 391,
	RPL_USERSSTART = 392,
	RPL_ENDOFUSERS = 394,
	RPL_NOUSERS = 395,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_WASNOSUCHNICK = 406,
	ERR_TOOMANYTARGETS = 407,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_WILDTOPLEVEL = 414,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NOMOTD = 422,
	ERR_NOADMININFO = 423,
	ERR_FILEERROR = 424,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOLOGIN = 444,
	ERR_SUMMONDISABLED = 445,
	ERR_USERSDISABLED = 446,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_NOPERMFORHOST = 463,
	ERR_PASSWDMISMATCH = 464,
	ERR_YOUREBANNEDCREEP = 465,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_CANTKILLSERVER = 483,
	ERR_NOOPERHOST = 491,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,
} ReplyCode;

typedef enum Modes {
	BANNED,
	NOTINCHANNEL,
	INVITED,
	USER,
	MODERATOR,
	OPERATOR,
	PROTECTED,
	FOUNDER
} Modes;

#define SERVERNAME "MiaoRC"
#define SERVERHOSTNAME "irc.125.outstanding.gov"
#define SERVERMAIL "miao@outstanding.gov"
#define SERVERVERSION "0.125.42b"
#define NETWORKNAME "Oustanding"
#define MOTD "Welcome to the IRC server"

// TODO find best constants
#define BACKLOG 128
#define MAX_CLIENTS 1024
#define MAX_EVENTS 32
#define BUFFER_SIZE 256
#define MAX_CHANNELS_PER_CLIENT 10
#define MAX_USERS_PER_CHANNEL 10

#define ARGUMENT_ERROR 2
#define OUTSTANDING_ERROR 125

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"

class SystemError : public std::runtime_error {
public:
	explicit SystemError(const char* funcName)
		: std::runtime_error(funcName), funcName(funcName) {}

	virtual ~SystemError() throw() {}

	const char* funcName;
};

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

// clean.cpp
void panic(std::string message);
void syscall(int returnValue, const char* funcName);
void signalHandler(int signum);

// utils.cpp
bool					 isPortNumberCorrect(std::string port);
bool					 isStringPrintable(std::string str);
std::string				 concatenateArguments(std::vector<std::string> arguments, unsigned int start);
std::string				 toLowerCase(std::string str);
std::string				 toUpperCase(std::string str);
std::string				 toString(Client* client);
std::string				 toString(long number);
std::string				 formatTime(time_t time);
std::vector<std::string> split(const std::string& str, char delim);
