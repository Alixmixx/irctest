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

std::string getCurrentDateTime()
{
    std::time_t currentTime = std::time(NULL);
    std::tm* localTime = std::localtime(&currentTime);

    // Array of day names
    const char* dayNames[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    // Array of month names
    const char* monthNames[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    std::stringstream dateTimeStream;
    dateTimeStream << dayNames[localTime->tm_wday] << " "
                   << monthNames[localTime->tm_mon] << " "
                   << std::setfill('0') << std::setw(2) << localTime->tm_mday << " "
                   << std::setw(2) << localTime->tm_hour << ":"
                   << std::setw(2) << localTime->tm_min << ":"
                   << std::setw(2) << localTime->tm_sec << " "
                   << (localTime->tm_year + 1900);

    return dateTimeStream.str();
}