#include "Server.hpp"

extern bool run;

void Server::handleDie(Client* client, std::vector<std::string> arguments)
{
    (void)arguments;
    if (!client->isAdmin())
        return client->reply(ERR_NOPRIVILEGES);

    run = false;
}