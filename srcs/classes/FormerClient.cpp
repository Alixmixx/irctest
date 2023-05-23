#include "FormerClient.hpp"

FormerClient::FormerClient(std::string nickname, std::string username, std::string realname,
						   time_t leaveTime, unsigned long ipAddress)
	: nickname(nickname), username(username), realname(realname), leaveTime(leaveTime),
	  ipAddress(ipAddress)
{
}

FormerClient::~FormerClient(){};
