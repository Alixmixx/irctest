#include "FormerClient.hpp"

FormerClient::FormerClient(std::string nickname, std::string username, std::string realname,
						   std::string hostname, time_t leaveTime)
	: nickname(nickname), username(username), realname(realname), hostname(hostname),
	  leaveTime(leaveTime)
{
}

FormerClient::~FormerClient(){};
