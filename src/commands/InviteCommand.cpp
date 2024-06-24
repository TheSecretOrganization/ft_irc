#include "commands/InviteCommand.hpp"
#include "Server.hpp"

InviteCommand::InviteCommand() : Command("INVITE", 2, 2) {}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute(Client* client, std::string args) {
	std::vector<std::string> vecArgs = Command::split(args, ' ');
	if (needMoreParams(client, vecArgs)) {
		return;
	}

	Channel* channel = Server::getInstance().getChannel(vecArgs[1]);
	if (noSuchChannel(client, channel, vecArgs[1])) {
		return;
	}

	if (notOnChannel(client, channel)) {
		return;
	}

	if (userOnChannel(client, channel, vecArgs[1])) {
		return;
	}

	Client* destinationClient = Server::getInstance().getClient(vecArgs[1]);

	// TODO: RPL_INVITING to client
	// and send an INVITE to destinationClient, how??
}
