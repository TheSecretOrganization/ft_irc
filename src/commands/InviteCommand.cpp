#include "commands/InviteCommand.hpp"
#include "Server.hpp"
#include "IrcReplies.hpp"
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
		sendError(client, ERR_NOSUCHNICK, _401, vecArgs[1]);
		return ;
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

	try
	{
		client->sendMessage(RPL_INVITING, client->getClientnickName() + " " + destinationClient->getClientnickName() + " " + channel->getChannelName());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	channel->inviteUser(destinationClient);

	try
	{
		destinationClient->sendMessage(":" + client->getClientnickName() + " INVITE", destinationClient->getClientnickName() + " " + channel->getChannelName());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
