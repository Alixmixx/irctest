#include "Server.hpp"

void Server::sendErrorMessage(int clientSocket, std::string errorName)
{
	std::string errorMessage = ircErrors[errorName].second;

	size_t pos;

	pos = errorMessage.find("<nick>"); // si on trouve <nick> dans le message d'erreur, on le remplace par le nickname du client
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 6, _clients[clientSocket].second.getNickname());
	}

	pos = errorMessage.find("<username>"); // si on trouve <username> dans le message d'erreur, on le remplace par le username du client
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 11, _clients[clientSocket].second.getUsername());
	}

	pos = errorMessage.find("<host>"); // etc...
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 6, _clients[clientSocket].second.getHost());
	}

	pos = errorMessage.find("<realname>");
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 10, _clients[clientSocket].second.getRealname());
	}

	pos = errorMessage.find("<networkname>");
	if (pos != std::string::npos)
	{
		errorMessage.replace(pos, 13, _serverName);
	}

	std::string fullMessage = _serverName + " " + errorMessage + "\r\n"; // on ajoute le nom du serveur au debut du message d'erreur et on ajoute le \r\n a la fin pour que le client sache que c'est la fin du message
	send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
}