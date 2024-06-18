#include "commands/InviteCommand.hpp"
#include "Server.hpp"

InviteCommand::InviteCommand()
{
}

InviteCommand::~InviteCommand()
{
}

void	InviteCommand::execute(Client* client, std::string args) {
	std::vector<std::string> vecArgs;
	Command::splitArgs(args, &vecArgs);
	if (vecArgs.size() != 3) {
		throw	MissingParamsInCommandException(client, "INVITE");
	}

	Channel* channel = Server::getInstance().getChannel(vecArgs[1]);
	if (channel == NULL) {
		throw	NoChannelException(client, "INVITE");
	}

	if (!channel->isUserOnChannel(client)) {
		throw	NotOnChannelException(client, "INVITE");
	}

	Client* destinationClient = Server::getInstance().getClient(vecArgs[1]);
	if (channel->isUserOnChannel(destinationClient)) {
		throw	UserOnChannelException(client, vecArgs[1], vecArgs[2]);
	}

	// TODO: RPL_INVITING to client
	// and send an INVITE to destinationClient, how??
}
