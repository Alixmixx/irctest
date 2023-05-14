#include "Server.hpp"

// Constructeur

Server::Server(std::string serverName, std::string serverInfo,
               std::string serverVersion, std::string serverEnvironment,
               std::string port)
    : _serverName(serverName), _serverInfo(serverInfo),
      _serverVersion(serverVersion), _serverEnvironment(serverEnvironment),
      _port(std::atoi(port.c_str())) {}

// Destructeur

Server::~Server() {}

// Setters

void Server::setServerName(std::string serverName) { _serverName = serverName; }

void Server::setServerInfo(std::string serverInfo) { _serverInfo = serverInfo; }

void Server::setServerVersion(std::string serverVersion) {
    _serverVersion = serverVersion;
}

void Server::setServerEnvironment(std::string serverEnvironment) {
    _serverEnvironment = serverEnvironment;
}

// Getters

std::string Server::getServerName() const { return (_serverName); }

std::string Server::getServerInfo() const { return (_serverInfo); }

std::string Server::getServerVersion() const { return (_serverVersion); }

std::string Server::getServerEnvironment() const {
    return (_serverEnvironment);
}

std::vector<Client> Server::getClients() const { return (_clients); }

// std::vector<Channel> Server::getChannels() const
// {
// 	return (_channels);
// }

// Statics

void Server::clean() {
    std::cout << "Cleaning..." << std::endl;
    for (int i = 0; i < MAX_CLIENTS; ++i)
        close(_evlist[i].data.fd);
    close(_socketFd);
    close(_epollFd);
}

void Server::goodBye() {
    std::cout << "\rGood bye. 💞\n";
    clean();
    exit(EXIT_SUCCESS);
}

void Server::handleSigint(int signum) {
    std::cout << "SIGINT received !" << std::endl;
    (void)signum;
    // goodBye();
}

void Server::syscall(int returnValue, const char *funcName) {
    if (returnValue == -1) {
        std::perror(funcName);
        clean();
        exit(EXIT_FAILURE);
    }
}

std::string Server::fullRead(int fd) {
    std::string message;
    char buf[BUFFER_SIZE_IRC];

    while (true) {
        int buflen;
        syscall(buflen = read(fd, buf, BUFFER_SIZE_IRC - 1), "read");
        buf[buflen] = '\0';
        message += buf;
        if (buflen < BUFFER_SIZE_IRC - 1)
            return message;
    }
}

void Server::addEvent(int epollFd, int eventFd) {
    struct epoll_event evStdin;

    evStdin.events = EPOLLIN;
    evStdin.data.fd = eventFd;
    syscall(epoll_ctl(epollFd, EPOLL_CTL_ADD, eventFd, &evStdin), "epoll_ctl");
}

void Server::initEpoll(char *port) {
    struct sockaddr_in serverSocket;
    bzero(&serverSocket, sizeof(serverSocket));
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(atoi(port));
    serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    syscall(_socketFd = socket(AF_INET, SOCK_STREAM, 0), "socket");
    syscall(
        bind(_socketFd, (struct sockaddr *)&serverSocket, sizeof(serverSocket)),
        "bind");
    syscall(listen(_socketFd, BACKLOG), "listen");
    syscall(_epollFd = epoll_create1(0), "epoll_create1");
    addEvent(_epollFd, STDIN_FILENO);
    addEvent(_epollFd, _socketFd);
}

void Server::loop() {
    int clientFd, numFds;
    while (true) {
        syscall(numFds = epoll_wait(_epollFd, _evlist, MAX_CLIENTS, -1),
                "epoll_wait");
        for (int i = 0; i < numFds; ++i) {
            if (_evlist[i].data.fd == STDIN_FILENO) {
                std::string input = fullRead(STDIN_FILENO);
                if (input == "quit\n")
                    goodBye();
            } else if (_evlist[i].data.fd == _socketFd) {
                struct sockaddr_in clientSocket;
                bzero(&clientSocket, sizeof(clientSocket));
                socklen_t socklen = sizeof(clientSocket);
                syscall(clientFd =
                            accept(_socketFd, (struct sockaddr *)&clientSocket,
                                   &socklen),
                        "accept");
                std::cout << "\x1b[0;32m[*] accept\x1b[0m\n";
                struct epoll_event ev;
                ev.events = EPOLLIN;
                ev.data.fd = clientFd;
                syscall(epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientFd, &ev),
                        "epoll_ctl");
            } else {
                clientFd = _evlist[i].data.fd;
                std::string msg = fullRead(clientFd);
                if (msg == "") {
                    std::cout << "\x1b[0;31m[*] close\x1b[0m\n";
                    syscall(epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd,
                                      &_evlist[i]),
                            "epoll_ctl");
                    close(clientFd);
                } else {
                    std::cout << msg << std::endl;
                }
            }
        }
    }
}