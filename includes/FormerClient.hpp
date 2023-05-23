#pragma once

#include "ft_irc.hpp"

class FormerClient {
public:
	FormerClient(std::string nickname, std::string username, std::string realname, time_t leaveTime,
				 unsigned long ipAddress);
	~FormerClient();

	const std::string	nickname;
	const std::string	username;
	const std::string	realname;
	const time_t		leaveTime;
	const unsigned long ipAddress;
};