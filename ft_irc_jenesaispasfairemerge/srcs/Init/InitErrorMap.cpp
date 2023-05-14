#include "Server.hpp"

void Server::initErrorMap()
{
	_errorMap["ERR_NOSUCHNICK"] = "401 <nickname> :No such nick/channel";
	_errorMap["ERR_NOSUCHSERVER"] = "402 <server name> :No such server";
	_errorMap["ERR_NOSUCHCHANNEL"] = "403 <channel name> :No such channel";
	_errorMap["ERR_CANNOTSENDTOCHAN"] = "404 <channel name> :Cannot send to channel";
	_errorMap["ERR_TOOMANYCHANNELS"] = "405 <channel name> :You have joined too many channels";
	_errorMap["ERR_WASNOSUCHNICK"] = "406 <nickname> :There was no such nickname";
	_errorMap["ERR_TOOMANYTARGETS"] = "407 <target> :<error code> recipients. <abort message>";
	_errorMap["ERR_NOORIGIN"] = "409 :No origin specified";
	_errorMap["ERR_NORECIPIENT"] = "411 :No recipient given (<command>)";
	_errorMap["ERR_NOTEXTTOSEND"] = "412 :No text to send";
	_errorMap["ERR_NOTOPLEVEL"] = "413 <mask> :No toplevel domain specified";
	_errorMap["ERR_WILDTOPLEVEL"] = "414 <mask> :Wildcard in toplevel domain";
	_errorMap["ERR_UNKNOWNCOMMAND"] = "421 <command> :Unknown command";
	_errorMap["ERR_NOMOTD"] = "422 :MOTD File is missing";
	_errorMap["ERR_NOADMININFO"] = "423 <server> :No administrative info available";
	_errorMap["ERR_FILEERROR"] = "424 :File error doing <file op> on <file>";
	_errorMap["ERR_NONICKNAMEGIVEN"] = "431 :No nickname given";
	_errorMap["ERR_ERRONEUSNICKNAME"] = "432 <nick> :Erroneus nickname";
	_errorMap["ERR_NICKNAMEINUSE"] = "433 <nick> :Nickname is already in use";
	_errorMap["ERR_NICKCOLLISION"] = "436 <nick> :Nickname collision KILL"; // pas besoin, c'est pour les serveurs multi normalemt
	_errorMap["ERR_USERNOTINCHANNEL"] = "441 <nick> <channel> :They aren't on that channel";
	_errorMap["ERR_NOTONCHANNEL"] = "442 <channel> :You're not on that channel";
	_errorMap["ERR_USERONCHANNEL"] = "443 <user> <channel> :is already on channel";
	_errorMap["ERR_NOLOGIN"] = "444 <user> :User not logged in";
	_errorMap["ERR_SUMMONDISABLED"] = "445 :SUMMON has been disabled";
	_errorMap["ERR_USERSDISABLED"] = "446 :USERS has been disabled";
	_errorMap["ERR_NOTREGISTERED"] = "451 :You have not registered";
	_errorMap["ERR_NEEDMOREPARAMS"] = "461 <command> :Not enough parameters";
	_errorMap["ERR_ALREADYREGISTRED"] = "462 :You may not reregister";
	_errorMap["ERR_NOPERMFORHOST"] = "463 :Your host isn't among the privileged";
	_errorMap["ERR_PASSWDMISMATCH"] = "464 :Password incorrect";
	_errorMap["ERR_YOUREBANNEDCREEP"] = "465 :You are banned from this server";
	_errorMap["ERR_KEYSET"] = "467 <channel> :Channel key already set";
	_errorMap["ERR_CHANNELISFULL"] = "471 <channel> :Cannot join channel (+l)";
	_errorMap["ERR_UNKNOWNMODE"] = "472 <char> :is unknown mode char to me";
	_errorMap["ERR_INVITEONLYCHAN"] = "473 <channel> :Cannot join channel (+i)";
	_errorMap["ERR_BANNEDFROMCHAN"] = "474 <channel> :Cannot join channel (+b)";
	_errorMap["ERR_BADCHANNELKEY"] = "475 <channel> :Cannot join channel (+k)";
	_errorMap["ERR_NOPRIVILEGES"] = "481 :Permission Denied- You're not an IRC operator";
	_errorMap["ERR_CHANOPRIVSNEEDED"] = "482 <channel> :You're not channel operator";
	_errorMap["ERR_CANTKILLSERVER"] = "483 :You cant kill a server!"; // lol
	_errorMap["ERR_NOOPERHOST"] = "491 :No O-lines for your host";
	_errorMap["ERR_UMODEUNKNOWNFLAG"] = "501 :Unknown MODE flag";
	_errorMap["ERR_USERSDONTMATCH"] = "502 :Cant change mode for other users";
};