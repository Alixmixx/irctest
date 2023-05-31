#include "Bot.hpp"
#include <curl/curl.h>

extern bool run;

Bot::Bot(std::string botName, std::string botPrompt, short _serverPort, std::string serverPassword)
	: _serverPort(_serverPort),
	  _botName(botName),
	  _botPrompt(botPrompt),
	  _serverPassword(serverPassword)
{
}

Bot::~Bot()
{
	std::cout << RED << "Bot " << _botName << " disconnected." << RESET << std::endl;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
	size_t totalSize = size * nmemb;
	output->append((char *)contents, totalSize);
	return totalSize;
}

std::string Bot::GetChatGPTResponse(const std::string &input)
{
	std::string response;

	// Initialize cURL
	CURL *curl = curl_easy_init();
	if (curl)
	{
		// Set the API endpoint URL
		const std::string apiUrl = "https://api.openai.com/v1/chat/completions";

		// Set the request headers
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(
			headers, "Authorization: Bearer sk-tb543qxcMOK7BO5Rqoj0T3BlbkFJOcxrAfRpiav24dNZiJL//V");

		// Set the request payload
		const std::string payload =
			"{\"messages\": [{\"role\": \"system\", \"content\": \"You can start the "
			"conversation.\"}, {\"role\": \"user\", \"content\": \"" +
			input + "\"}], \"max_tokens\": 100, \"model\": \"gpt-3.5-turbo\"}";

		// Set cURL options
		curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// Perform the request
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

		// Clean up
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}

	return response;
}

void Bot::sendBotInit()
{
	std::string login;

	login = "CAP LS\r\nPASS " + _serverPassword + "\r\nNICK " + _botName + "\r\nUSER " +
			_botName + " 0 *: bot\r\n ";

	send(_botSocket, login.c_str(), login.length(), 0);
}

void *threadBot(void *botPtr)
{
	Bot *bot = static_cast<Bot *>(botPtr);

	bot->runBot();
	pthread_exit(NULL);
}

void Bot::runBot()
{
	_botSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_botSocket == -1)
	{
		std::cerr << "Failed to create socket" << std::endl;
		return;
	}

	struct sockaddr_in botAddress;
	botAddress.sin_family = AF_INET;
	botAddress.sin_port = htons(_serverPort);

	syscall(inet_pton(AF_INET, "127.0.0.1", &(botAddress.sin_addr)), "Invalid server IP address");

	syscall(connect(_botSocket, (struct sockaddr *)&botAddress, sizeof(botAddress)),
			"Invalid server IP address");

	sendBotInit();

	char buffer[BUFFER_SIZE] = {0};
	while (true)
	{
		std::fill(buffer, buffer + BUFFER_SIZE, 0);
		if (recv(_botSocket, buffer, BUFFER_SIZE, 0) <= 0)
		{
			std::cerr << "Connection closed by server" << std::endl;
			return;
		}

		// Check if message is a PRIVMSG
		std::string clientMessage(buffer);
		if (clientMessage == "Botshutdown\r\n")
		{
			close(_botSocket);
			return;
		}

		if (clientMessage.find("PRIVMSG " + _botName) == std::string::npos)
			continue;

		// Get client nickname
		std::string clientNickname(buffer);
		clientNickname = clientNickname.substr(1, clientNickname.find("!") - 1);

		// Get request from message
		std::string prefix = "PRIVMSG " + _botName + " :";
		std::string request = clientMessage.substr(clientMessage.find(prefix) + prefix.length());
		request = request.substr(0, request.find("\r\n"));
		for (size_t i = 0; i < request.length(); i++)
		{
			if (request[i] == '\"' || request[i] == '\'' || request[i] == '\\')
			{
				request.replace(i, 1, "");
				i--;
			}
		}
		if (request.empty())
			continue;

		std::cout << "Request from " << clientNickname << " : " << request << std::endl;
		std::string responseGPT = GetChatGPTResponse(_botPrompt + request);

		// Need to parse responseGPT to get the response and errors, not done yet lorenzo
		if (responseGPT.find("error") != std::string::npos)
		{
			responseGPT = "Error : could not connect to GPT-3 API";
		}
		else
		{
			responseGPT = responseGPT.substr(responseGPT.find("content") + 10);
			responseGPT = responseGPT.substr(0, responseGPT.find("\""));
		}

		std::string response = "PRIVMSG " + clientNickname + " :" + responseGPT + "\r\n";

		if (send(_botSocket, response.c_str(), response.length(), 0) <= 0)
		{
			std::cerr << "Failed to send data to server" << std::endl;
			close(_botSocket);
			return;
		}
	}
	return;
}
