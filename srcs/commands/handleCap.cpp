#include "Server.hpp"

// Clients implementing capability negotiation will still interoperate with servers that do not implement it;
// similarly, servers that implement capability negotiation will successfully communicate with clients that do not implement it.
// https://modern.ircdocs.horse/#capability-negotiation

void Server::handleCap(Client* client, std::vector<std::string> arguments)
{
	(void)client;
	(void)arguments;
}