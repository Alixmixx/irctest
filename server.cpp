#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#define BACKLOG 128
#define BUFFER_SIZE_IRC 1024
#define MAX_CLIENTS 1024

int fd;
struct epoll_event evlist[MAX_CLIENTS];

static void clean() {
    for (int i = 0; i < MAX_CLIENTS; ++i)
        close(evlist[i].data.fd);
    close(fd);
}

static void goodBye() {
    std::cout << "\rGood bye. 💞\n";
    clean();
    exit(EXIT_SUCCESS);
}

static void handleSigint(int signum) {
    (void)signum;
    goodBye();
}

static void syscall(int returnValue, const char *funcName) {
    if (returnValue == -1) {
        perror(funcName);
        clean();
        exit(EXIT_FAILURE);
    }
}

static std::string fullRead(int fd) {
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

int main(int argc, char **argv) {
    static socklen_t socklen = sizeof(struct sockaddr);
    int cfd, epfd, nfds;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port\n";
        return EXIT_FAILURE;
    }
    signal(SIGINT, handleSigint);
    struct sockaddr_in serverSocket, clientSocket;
    memset(&serverSocket, 0, socklen);
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(atoi(argv[1]));
    serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    syscall(fd = socket(AF_INET, SOCK_STREAM, 0), "socket");
    syscall(bind(fd, (struct sockaddr *)&serverSocket, socklen), "bind");
    syscall(listen(fd, BACKLOG), "listen");
    syscall(epfd = epoll_create1(0), "epoll_create1");
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    syscall(epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev), "epoll_ctl");
    ev.data.fd = fd;
    syscall(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev), "epoll_ctl");

    std::cout << "Listening on port " << argv[1] << ". 👂\n";

    while (true) {
        syscall(nfds = epoll_wait(epfd, evlist, MAX_CLIENTS, -1), "epoll_wait");
        for (int i = 0; i < nfds; ++i) {
            if (evlist[i].data.fd == STDIN_FILENO) {
                std::string input = fullRead(STDIN_FILENO);
                if (input == "quit\n")
                    goodBye();
            } else if (evlist[i].data.fd == fd) {
                syscall(accept(fd, (struct sockaddr *)&clientSocket, &socklen),
                        "accept");
                std::cout << "\x1b[0;32m[*] accept\x1b[0m\n";
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                syscall(epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev), "epoll_ctl");
            } else {
                cfd = evlist[i].data.fd;
                std::string msg = fullRead(cfd);
                if (msg == "") {
                    std::cout << "\x1b[0;31m[*] close\x1b[0m\n";
                    syscall(epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, &evlist[i]),
                            "epoll_ctl");
                    close(cfd);
                } else {
                    std::cout << msg << std::endl;
                }
            }
        }
    }
}