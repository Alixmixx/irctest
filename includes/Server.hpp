#include "Server.hpp"

#include <iostream>
#include <string>
#include <vector>

class Server
{
	public:
		Server(std::string serverName, std::string serverInfo, std::string serverVersion, std::string serverEnvironment);
		~Server();

		// Setters
		void setServerName(std::string serverName);
		void setServerInfo(std::string serverInfo);
		void setServerVersion(std::string serverVersion);
		void setServerEnvironment(std::string serverEnvironment);
		// Getters
		std::string getServerName() const;
		std::string getServerInfo() const;
		std::string getServerVersion() const;
		std::string getServerEnvironment() const;
		std::vector<Client> getClients() const;
		std::vector<Channel> getChannels() const;
		// Methods
		void addClient(Client client);
		void removeClient(Client client);
		void addChannel(Channel channel);
		void removeChannel(Channel channel);
		void sendErrorMessage(int socket, std::string errorName);

	private:
		std::string _serverName;
		std::string _serverInfo;
		std::string _serverVersion;
		std::string _serverEnvironment;
		std::vector<Client> _clients;
		std::vector<Channel> _channels;
}