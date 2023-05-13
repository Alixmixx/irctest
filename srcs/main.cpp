#include "Client.hpp"
#include "Server.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port\n";
        return EXIT_FAILURE;
    }
    Server server("MyServer", "1.0", "dev", "env", argv[1]);
    // std::signal(SIGINT, Server::handleSigint);
    server.initEpoll(argv[1]);
    std::cout << "Listening on port " << argv[1] << ". 👂\n";
    server.loop();
}