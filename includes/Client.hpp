#include "Client.hpp"
#include "Server.hpp"

class Client
{
public:
	Client(int socket, std::string nickname, std::string username, std::string realname, std::string hostname);
	~Client();

	// Setters
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setHostname(std::string hostname);
	// Getters
	std::string getNickname() const;
	std::string getUsername() const;
	std::string getRealname() const;
	std::string getHostname() const;
	int getSocket() const;
	Server *getServer() const;
	void setServer(Server *server);

private:
	int _socket;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _hostname;
	Server *_server;
};
