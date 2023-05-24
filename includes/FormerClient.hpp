#pragma once

#include "ft_irc.hpp"

class FormerClient {
public:
	FormerClient(std::string nickname, std::string username, std::string realname,
				 std::string hostname, time_t leaveTime);
	~FormerClient();

	const std::string nickname;
	const std::string username;
	const std::string realname;
	const std::string hostname;
	const time_t	  leaveTime;
};