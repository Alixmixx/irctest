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

#define MAX_CLIENTS 1024
#define BUFFER_SIZE_IRC 8192

int fd;
struct epoll_event evlist[MAX_CLIENTS];

static void goodBye() {
    std::cout << "\rGood bye. 💞\n";
    for (int i = 0; i < MAX_CLIENTS; ++i)
        close(evlist[i].data.fd);
    close(fd);
    exit(EXIT_SUCCESS);
}

static void handleSigint(int signum) {
    (void)signum;
    goodBye();
}

static void syscall(int returnValue, const char *funcName) {
    if (returnValue == -1) {
        perror(funcName);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    static socklen_t socklen = sizeof(struct sockaddr);
    int buflen, cfd, epfd, nfds;
    char buf[BUFFER_SIZE_IRC];
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
    syscall(listen(fd, 5), "listen");
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
                syscall(buflen = read(STDIN_FILENO, buf, BUFFER_SIZE_IRC - 1),
                        "read");
                buf[buflen] = '\0';
                if (!strcmp(buf, "quit\n"))
                    goodBye();
            } else if (evlist[i].data.fd == fd) {
                cfd = accept(fd, (struct sockaddr *)&clientSocket, &socklen);
                if (cfd == -1) {
                    perror("accept");
                    exit(1);
                }
                printf("\x1b[0;32m[*] accept\x1b[0m\n");
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev) == -1) {
                    perror("epoll_ctl");
                    exit(1);
                }
            } else {
                cfd = evlist[i].data.fd;
                syscall(buflen = read(cfd, buf, BUFFER_SIZE_IRC - 1), "read");
                if (buflen == 0) {
                    std::cout << "\x1b[0;31m[*] close\x1b[0m\n";
                    syscall(epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, &evlist[i]),
                            "epoll_ctl");
                    close(cfd);
                } else {
                    buf[buflen] = '\0';
                    std::cout << buf << std::endl;
                }
            }
        }
    }
}
