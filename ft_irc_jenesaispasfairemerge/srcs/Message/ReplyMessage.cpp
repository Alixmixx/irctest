#include "Server.hpp"

void Server::sendReplyMessage(Client *client, std::string replyCode)
{
	const std::map<std::string, std::string> &replyMap = client->getServer()->getReplyMap();
	std::map<std::string, std::string>::const_iterator it = replyMap.find(replyCode);
	std::string replyMessage;

	if (it != replyMap.end())
	{
		replyMessage = it->second;
	}
	else
	{
		replyMessage = "Error: Reply code not found."; // si on ne trouve pas le code de reponse dans la map, on envoie un message d'erreur, mais ca ne devrait jamais arriver
	}

	std::cout << "Reply message: " << replyMessage << std::endl;

	size_t pos;

	pos = replyMessage.find("<nick>"); // si on trouve <nick> dans le message d'erreur, on le remplace par le nickname du client
	if (pos != std::string::npos)
	{
		replyMessage.replace(pos, 6, client->getNickname());
	}

	pos = replyMessage.find("<user>"); // si on trouve <username> dans le message d'erreur, on le remplace par le username du client
	if (pos != std::string::npos)
	{
		replyMessage.replace(pos, 10, client->getUsername());
	}

	pos = replyMessage.find("<host>"); // etc...
	if (pos != std::string::npos)
	{
		replyMessage.replace(pos, 6, client->getHostname());
	}

	pos = replyMessage.find("<realname>");
	if (pos != std::string::npos)
	{
		replyMessage.replace(pos, 10, client->getRealname());
	}

	pos = replyMessage.find("<networkname>");
	if (pos != std::string::npos)
	{
		replyMessage.replace(pos, 13, _serverName);
	}

	std::string fullMessage = _serverName + " " + replyMessage + "\r\n"; // on ajoute le nom du serveur au debut du message d'erreur et on ajoute le \r\n a la fin pour que le client sache que c'est la fin du message

	std::cout << "Reply message: " << fullMessage << std::endl;

	send(client->getSocket(), fullMessage.c_str(), fullMessage.length(), 0);
}