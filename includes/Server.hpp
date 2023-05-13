#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

#include <arpa/inet.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>

#define BACKLOG 128
#define MAX_CLIENTS 1024
#define BUFFER_SIZE_IRC 1024

class Client;

class Server {
  public:
    Server(std::string serverName, std::string serverInfo,
           std::string serverVersion, std::string serverEnvironment,
           std::string port);
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
    // std::vector<Channel> getChannels() const;

    // Statics
    void clean();
    void goodBye();
    void handleSigint(int signum);
    void syscall(int returnValue, const char *funcName);
    std::string fullRead(int fd);
    void addEvent(int epollFd, int eventFd);
    void initEpoll(char *port);
    void loop();

    // Methods
    void addClient(Client client);
    void removeClient(Client client);
    // void addChannel(Channel channel);
    // void removeChannel(Channel channel);

  private:
    int _epollFd;
    int _socketFd;
    struct epoll_event _evlist[MAX_CLIENTS];
    std::string _serverName;
    std::string _serverInfo;
    std::string _serverVersion;
    std::string _serverEnvironment;
    std::vector<Client> _clients;
    short _port;
    // std::vector<Channel>	_channels;

    // Server infos
    const std::string _serverPassword;
    const std::string _serverCreationDate;
    const std::string _serverCreationTime;
};

#endif