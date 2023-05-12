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

int nfds;
struct sockaddr_in ssin, csin;
socklen_t socklen = sizeof(struct sockaddr);
struct epoll_event ev;
struct epoll_event evlist[MAX_CLIENTS];

void handleSigint(int signum) {
    (void)signum;
    std::cout << "\rExit by interrupt signal" << std::endl;
    for (int i = 0; i < nfds; ++i)
        close(evlist[i].data.fd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int ret, fd, cfd, epfd;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port\n";
        return EXIT_FAILURE;
    }
    signal(SIGINT, handleSigint);
    memset(&ssin, 0, socklen);
    ssin.sin_family = AF_INET;
    ssin.sin_port = htons(atoi(argv[1]));
    ssin.sin_addr.s_addr = htonl(INADDR_ANY);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(1);
    }
    if (bind(fd, (struct sockaddr *)&ssin, socklen) == -1) {
        perror("bind");
        exit(1);
    }
    if (listen(fd, 5) == -1) {
        perror("listen");
        exit(1);
    }

    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(1);
    }
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

    while (true) {
        nfds = epoll_wait(epfd, evlist, MAX_CLIENTS, -1);
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
                cfd = accept(fd, (struct sockaddr *)&csin, &socklen);
                if (cfd == -1) {
                    perror("accept");
                    exit(1);
                }
                printf("\x1b[0;32m[*] accept\x1b[0m\n");
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                if (ret == -1) {
                    perror("epoll_ctl");
                    exit(1);
                }
            } else {
                cfd = evlist[i].data.fd;
                int buflen = read(cfd, buf, BUFFER_SIZE_IRC - 1);
                if (buflen == 0) {
                    close(cfd);
                    if (cfd == -1) {
                        perror("close");
                        exit(1);
                    }
                    printf("\x1b[0;31m[*] close\x1b[0m\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, &evlist[i]);
                    if (ret == -1) {
                        perror("epoll_ctl");
                        exit(1);
                    }
                } else {
                    buf[buflen] = '\0';
                    std::string msgPrefix =
                        "some-prefix-to-prevent-arbitrary-connection";
                    std::string msg = buf;

                    if (msgPrefix.length() > msg.length())
                        continue;
                    if (!strcmp(msgPrefix.c_str(),
                                msg.substr(0, msgPrefix.length()).c_str())) {
                        msg = msg.substr(msgPrefix.length(), msg.length());
                        std::cout << msg << std::endl;
                    }
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
