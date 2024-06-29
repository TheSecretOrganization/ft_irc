#include "commands/InviteCommand.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"
#include <iostream>

InviteCommand::InviteCommand() : Command("INVITE", 2, 2) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(Client* client, std::string args) {
	std::vector<std::string> vecArgs = Command::split(args, ' ');
	if (needMoreParams(client, vecArgs)) {
		return;
	}

	Client* destinationClient = Server::getInstance().getClient(vecArgs[1]);
	if (!destinationClient) {
		return client->sendError(ERR_NOSUCHNICK,
								 client->getClientnickName() + " " + vecArgs[1],
								 _401);
	}

	Channel* channel = Server::getInstance().getChannel(vecArgs[1]);
	if (noSuchChannel(client, channel, vecArgs[1])) {
		return;
	}

	if (notOnChannel(client, channel)) {
		return;
	}

	if (chanOPrivsNeeded(client, channel)) {
		return;
	}

	if (userOnChannel(client, channel, vecArgs[1])) {
		return;
	}

	try {
		client->sendMessage(Server::getInstance().getPrefix(), RPL_INVITING,
							client->getClientnickName() + " " +
								destinationClient->getClientnickName() + " " +
								channel->getName());
	} catch (const ClientSocket::SendException& e) {
		std::cerr << e.what() << '\n';
	}

	if (!channel->isUserInvited(destinationClient))
		channel->inviteUser(destinationClient);

	try {
		destinationClient->sendMessage(client->getPrefix(), "INVITE",
									   destinationClient->getClientnickName(),
									   channel->getName());
	} catch (const ClientSocket::SendException& e) {
		std::cerr << e.what() << '\n';
	}
}
