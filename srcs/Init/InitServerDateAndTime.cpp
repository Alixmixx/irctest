#include "Server.hpp"

void Server::initServerDateAndTime()
{
	std::time_t currentTime = std::time(NULL);
	std::tm *localTime = std::localtime(&currentTime);

	std::ostringstream dateStream;
	dateStream << std::setfill('0') << std::setw(4) << (localTime->tm_year + 1900) << "-"
			   << std::setw(2) << (localTime->tm_mon + 1) << "-" << std::setw(2) << localTime->tm_mday;
	_serverCreationDate = dateStream.str();

	std::ostringstream timeStream;
	timeStream << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":"
			   << std::setw(2) << localTime->tm_min << ":" << std::setw(2) << localTime->tm_sec;
	_serverCreationTime = timeStream.str();
}

