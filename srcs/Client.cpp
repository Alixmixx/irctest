#include "Server.hpp"
#include "Client.hpp"

static bool isNicknameValid(std::string nickname)
{
	if (nickname.length() > 9)
		return false;
	if (nickname.find_first_not_of(" ,*?!@") != std::string::npos)
		return false;
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#' || nickname[0] == '&')
		return false;
	return true;
}

static bool isNicknameAlreadyTaken(std::string nickname)
{
	std::vector<Client>::iterator it;
	for (it = _server->_clients.begin(); it != _server->_clients.end(); it++)
	{
		if (it->second.getNickname().toLower() == nickname.toLower()) // toLower() nickname in irc are case insensitive
			return true;
	}
	return false;
}

void Client::setNickname(std::string nickname)
{
	if (nickname.empty())
	{
		_server->sendErrorMessage(this->_socket, "ERR_NONICKNAMEGIVEN");
		return;
	}
	if (isNicknameValid(nickname))
	{
		if (isNicknameAlreadyTaken(nickname))
		{
			_server->sendErrorMessage(this->_socket, "ERR_NICKNAMEINUSE");
			return;
		}
		_nickname = nickname;
	}
	else
	{
		_server->sendErrorMessage(this->_socket, "ERR_ERRONEUSNICKNAME");
		return;
	}
}
