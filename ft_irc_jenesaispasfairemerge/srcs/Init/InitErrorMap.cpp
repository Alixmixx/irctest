#include "Server.hpp"

void Server::initErrorMap()
{
	_errorMap.insert(std::make_pair("ERR_NOSUCHNICK", "401 <nickname> :No such nick/channel"));
	_errorMap.insert(std::make_pair("ERR_NOSUCHSERVER", "402 <server name> :No such server"));
	_errorMap.insert(std::make_pair("ERR_NOSUCHCHANNEL", "403 <channel name> :No such channel"));
	_errorMap.insert(std::make_pair("ERR_CANNOTSENDTOCHAN", "404 <channel name> :Cannot send to channel"));
	_errorMap.insert(std::make_pair("ERR_TOOMANYCHANNELS", "405 <channel name> :You have joined too many channels"));
	_errorMap.insert(std::make_pair("ERR_WASNOSUCHNICK", "406 <nickname> :There was no such nickname"));
	_errorMap.insert(std::make_pair("ERR_TOOMANYTARGETS", "407 <target> :<error code> recipients. <abort message>"));
	_errorMap.insert(std::make_pair("ERR_NOORIGIN", "409 :No origin specified"));
	_errorMap.insert(std::make_pair("ERR_NORECIPIENT", "411 :No recipient given (<command>)"));
	_errorMap.insert(std::make_pair("ERR_NOTEXTTOSEND", "412 :No text to send"));
	_errorMap.insert(std::make_pair("ERR_NOTOPLEVEL", "413 <mask> :No toplevel domain specified"));
	_errorMap.insert(std::make_pair("ERR_WILDTOPLEVEL", "414 <mask> :Wildcard in toplevel domain"));
	_errorMap.insert(std::make_pair("ERR_UNKNOWNCOMMAND", "421 <command> :Unknown command"));
	_errorMap.insert(std::make_pair("ERR_NOMOTD", "422 :MOTD File is missing"));
	_errorMap.insert(std::make_pair("ERR_NOADMININFO", "423 <server> :No administrative info available"));
	_errorMap.insert(std::make_pair("ERR_FILEERROR", "424 :File error doing <file op> on <file>"));
	_errorMap.insert(std::make_pair("ERR_NONICKNAMEGIVEN", "431 :No nickname given"));
	_errorMap.insert(std::make_pair("ERR_ERRONEUSNICKNAME", "432 <nick> :Erroneus nickname"));
	_errorMap.insert(std::make_pair("ERR_NICKNAMEINUSE", "433 <nick> :Nickname is already in use"));
	_errorMap.insert(std::make_pair("ERR_NICKCOLLISION", "436 <nick> :Nickname collision KILL")); // pas besoin, c'est pour les serveurs multi normalement
	_errorMap.insert(std::make_pair("ERR_USERNOTINCHANNEL", "441 <nick> <channel> :They aren't on that channel"));
	_errorMap.insert(std::make_pair("ERR_NOTONCHANNEL", "442 <channel> :You're not on that channel"));
	_errorMap.insert(std::make_pair("ERR_USERONCHANNEL", "443 <user> <channel> :is already on channel"));
	_errorMap.insert(std::make_pair("ERR_NOLOGIN", "444 <user> :User not logged in"));
	_errorMap.insert(std::make_pair("ERR_SUMMONDISABLED", "445 :SUMMON has been disabled"));
	_errorMap.insert(std::make_pair("ERR_USERSDISABLED", "446 :USERS has been disabled"));
	_errorMap.insert(std::make_pair("ERR_NOTREGISTERED", "451 :You have not registered"));
	_errorMap.insert(std::make_pair("ERR_NEEDMOREPARAMS", "461 <command> :Not enough parameters"));
	_errorMap.insert(std::make_pair("ERR_ALREADYREGISTRED", "462 :You may not reregister"));
	_errorMap.insert(std::make_pair("ERR_NOPERMFORHOST", "463 :Your host isn't among the privileged"));
	_errorMap.insert(std::make_pair("ERR_PASSWDMISMATCH", "464 :Password incorrect"));
	_errorMap.insert(std::make_pair("ERR_YOUREBANNEDCREEP", "465 :You are banned from this server"));
	_errorMap.insert(std::make_pair("ERR_KEYSET", "467 <channel> :Channel key already set"));
	_errorMap.insert(std::make_pair("ERR_CHANNELISFULL", "471 <channel> :Cannot join channel (+l)"));
	_errorMap.insert(std::make_pair("ERR_UNKNOWNMODE", "472 <char> :is unknown mode char to me"));
	_errorMap.insert(std::make_pair("ERR_INVITEONLYCHAN", "473 <channel> :Cannot join channel (+i)"));
	_errorMap.insert(std::make_pair("ERR_BANNEDFROMCHAN", "474 <channel> :Cannot join channel (+b)"));
	_errorMap.insert(std::make_pair("ERR_BADCHANNELKEY", "475 <channel> :Cannot join channel (+k)"));
	_errorMap.insert(std::make_pair("ERR_NOPRIVILEGES", "481 :Permission Denied- You're not an IRC operator"));
	_errorMap.insert(std::make_pair("ERR_CHANOPRIVSNEEDED", "482 <channel> :You're not channel operator"));
	_errorMap.insert(std::make_pair("ERR_CANTKILLSERVER", "483 :You cant kill a server!")); // lol
	_errorMap.insert(std::make_pair("ERR_NOOPERHOST", "491 :No O-lines for your host"));
	_errorMap.insert(std::make_pair("ERR_UMODEUNKNOWNFLAG", "501 :Unknown MODE flag"));
	_errorMap.insert(std::make_pair("ERR_USERSDONTMATCH", "502 :Cant change mode for other users"));
};