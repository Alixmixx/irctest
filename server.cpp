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

struct epoll_event evlist[MAX_CLIENTS];

static void handleSigint(int signum) {
    (void)signum;
    std::cout << "\r💞 Good bye. 💞\n";
    for (int i = 0; i < MAX_CLIENTS; ++i)
        close(evlist[i].data.fd);
    exit(EXIT_SUCCESS);
}

static void syscall(int returnValue, const char *funcName) {
    if (returnValue == -1) {
        perror(funcName);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    static socklen_t socklen = sizeof(struct sockaddr);
    int cfd, epfd;
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

    const int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(1);
    }
    syscall(bind(fd, (struct sockaddr *)&serverSocket, socklen), "bind");
    syscall(listen(fd, 5), "listen");
    syscall(epfd = epoll_create1(0), "epoll_create1");
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }

    std::cout << "👂 Listening on port " << argv[1] << ". 👂\n";

    while (true) {
        const int nfds = epoll_wait(epfd, evlist, MAX_CLIENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < nfds; ++i) {
            char buf[BUFFER_SIZE_IRC];
            if (evlist[i].data.fd == STDIN_FILENO) {
                fgets(buf, BUFFER_SIZE_IRC - 1, stdin);
                if (!strcmp(buf, "quit") || !strcmp(buf, "exit")) {
                    close(fd);
                    exit(0);
                }
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
                int buflen = read(cfd, buf, BUFFER_SIZE_IRC - 1);
                if (buflen == 0) {
                    printf("\x1b[0;31m[*] close\x1b[0m\n");
                    if (epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, &evlist[i]) == -1) {
                        perror("epoll_ctl");
                        exit(1);
                    }
                    if (close(cfd) == -1) {
                        perror("close");
                        exit(1);
                    }
                } else {
                    buf[buflen] = '\0';
                    std::string msg = buf;
                    std::cout << msg << std::endl;
                }
            }
        }
    }
}
