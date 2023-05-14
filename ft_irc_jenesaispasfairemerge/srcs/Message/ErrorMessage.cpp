#include "Server.hpp"

void Server::sendErrorMessage(Client *client, std::string errorCode)
{
	std::string errorMessage = client->getServer()->getErrorMap()[errorCode];

	size_t pos;

	pos = errorMessage.find("<nick>"); // si on trouve <nick> dans le message d'erreur, on le remplace par le nickname du client
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 6, client->getNickname());
	}

	pos = errorMessage.find("<username>"); // si on trouve <username> dans le message d'erreur, on le remplace par le username du client
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 10, client->getUsername());
	}

	pos = errorMessage.find("<host>"); // etc...
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 6, client->getHostname());
	}

	pos = errorMessage.find("<realname>");
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 10, client->getRealname());
	}

	pos = errorMessage.find("<networkname>");
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 13, _serverName);
	}

	std::string fullMessage = _serverName + " " + errorMessage + "\r\n"; // on ajoute le nom du serveur au debut du message d'erreur et on ajoute le \r\n a la fin pour que le client sache que c'est la fin du message
	send(client->getSocket(), fullMessage.c_str(), fullMessage.length(), 0);
}