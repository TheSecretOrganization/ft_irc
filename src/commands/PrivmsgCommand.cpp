#include "commands/PrivmsgCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <string>

PrivmsgCommand::PrivmsgCommand() : Command("PRIVMSG", 0, 2) {}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute(Client* client, const std::string& args) {
	size_t i = args.find(":");

	if (args.empty() || i == 0) {
		client->sendError(ERR_NORECIPIENT, client->getClientnickName(), _411);
		return;
	}

	std::string target = args.substr(0, i);
	std::string message = (i != std::string::npos) ? args.substr(i + 1, args.size() - (i + 1))
									: "";

	if (message.empty()) {
		client->sendError(ERR_NOTEXTTOSEND, client->getClientnickName(), _412);
		return;
	}

	target = trim(target);
	if (target[0] == '#') {
		Channel* chan = Server::getInstance().getChannel(target);

		if (!chan)
			return client->sendError(ERR_NOSUCHNICK,
									 client->getClientnickName() + " " + target,
									 _401);

		if (chan->isInviteMode() && !chan->isUserOnChannel(client))
			return client->sendError(ERR_CANNOTSENDTOCHAN,
									 client->getClientnickName() + " " + target,
									 _404);

		chan->broadcast(client->getPrefix(), "PRIVMSG", message);
	} else {
		Client const* targetClient = Server::getInstance().getClient(target);

		if (!targetClient)
			return client->sendError(ERR_NOSUCHNICK,
									 client->getClientnickName() + " " + target,
									 _401);

		if (targetClient->isAway())
			return client->sendMessage(
				Server::getInstance().getPrefix(), RPL_AWAY,
				client->getClientnickName() + " " + targetClient->getNickname(),
				_301);

		targetClient->sendMessage(client->getPrefix(), "PRIVMSG", target, message);
	}
}
