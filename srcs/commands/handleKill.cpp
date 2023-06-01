#include "Server.hpp"

void Server::handleKill(Client* client, std::vector<std::string> arguments)
{
    if (arguments.size() < 2)
        return client->reply(ERR_NEEDMOREPARAMS, "KILL");

    if (!client->isAdmin())
        return client->reply(ERR_NOPRIVILEGES);

    Client* target = getClient(arguments[0]);
    if (!target)
        return client->reply(ERR_NOSUCHNICK, arguments[0]);

    target->reply("ERROR :Closing Link: " + _serverName + " 💣 Killed by " + client->getNickname() + " [" + arguments[1] + "]");
    arguments.push_back(KILL);
    arguments[0] = client->getNickname();
    handleQuit(target, arguments);
}