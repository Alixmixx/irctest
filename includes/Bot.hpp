#pragma once

#include "Server.hpp"
#include <curl/curl.h>

class Bot
{
	public:
		Bot(std::string botName, std::string botPrompt, short _serverPort, std::string serverPassword);
		~Bot();
		void runBot();
		void sendBotInit();
		std::string GetChatGPTResponse(const std::string& input);
		int getBotSocket() const;

	private:
		int _botSocket;
		short _serverPort;
		std::string _botName;
		std::string _botPrompt;
		std::string _serverPassword;
		pthread_t _threadBot;
};

void *threadBot(void *botPtr);