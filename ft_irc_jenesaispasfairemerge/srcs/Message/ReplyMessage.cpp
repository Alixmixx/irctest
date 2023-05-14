#include "Server.hpp"

static void replaceReplyMessageArguments(std::string toReplace, std::string replacement, std::string &replyMessage)
{
	size_t pos;

	pos = replyMessage.find(toReplace);
	if (pos != std::string::npos)
	{
		replyMessage.erase(pos, toReplace.length());
		replyMessage.insert(pos, replacement);
	}
}

void Server::sendReplyMessage(Client *client, std::string replyCode)
{
	std::string replyMessage = client->getServer()->getReplyMap()[replyCode];

	std::cout << "replyMessage: " << replyMessage << std::endl;

	replaceReplyMessageArguments("<nick>", client->getNickname(), replyMessage);
	replaceReplyMessageArguments("<user>", client->getUsername(), replyMessage);
	replaceReplyMessageArguments("<realname>", client->getRealname(), replyMessage);
	replaceReplyMessageArguments("<host>", client->getHostname(), replyMessage);
	replaceReplyMessageArguments("<networkname>", _serverName, replyMessage);
	replaceReplyMessageArguments("<servername>", _serverName, replyMessage);
	replaceReplyMessageArguments("<version>", _serverVersion, replyMessage);
	replaceReplyMessageArguments("<date>", _serverCreationDate, replyMessage);
	replaceReplyMessageArguments("<time>", _serverCreationTime, replyMessage);
	replaceReplyMessageArguments("<available user modes>", "user modes", replyMessage);
	replaceReplyMessageArguments("<available channel modes>", "channel modes", replyMessage);
	replaceReplyMessageArguments("<1-13 tokens>", "myTokens, not YOURS", replyMessage);

	std::string fullMessage = ":127.0.0.1 " + replyMessage + "\r\n"; // on ajoute le nom du serveur au debut du message d'erreur et on ajoute le \r\n a la fin pour que le client sache que c'est la fin du message

	send(client->getSocket(), fullMessage.c_str(), fullMessage.length(), 0);
}