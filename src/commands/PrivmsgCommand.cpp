#include "commands/PrivmsgCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

PrivmsgCommand::PrivmsgCommand() : Command("PRIVMSG", 0, 2) {}

PrivmsgCommand::~PrivmsgCommand() {}

static std::string trim(const std::string& str) {
	std::string::size_type start = 0;
	std::string::size_type end = str.size();

	while (start < end && std::isspace(str[start])) {
		++start;
	}

	if (start < end) {
		do {
			--end;
		} while (end > start && std::isspace(str[end]));
		++end;
	}

	return str.substr(start, end - start);
}

void PrivmsgCommand::execute(Client* client, std::string args) {
	size_t i = args.find(":");

	if (args.empty() || i == 0) {
		sendError(client, ERR_NORECIPIENT, _411);
		return;
	}

	std::string target = args.substr(0, i);
	args = (i != args.npos) ? args.substr(i + 1, args.size() - (i + 1)) : "";

	if (args.empty()) {
		sendError(client, ERR_NOTEXTTOSEND, _412);
		return;
	}

	target = trim(target);
	if (target[0] == '#') {
		Channel* chan = Server::getInstance().getChannel(target);

		if (!chan) {
			sendError(client, ERR_NOSUCHNICK, _401, target);
			return;
		}

		if (chan->isInviteMode() && !chan->isUserOnChannel(client)) {
			sendError(client, ERR_CANNOTSENDTOCHAN, _404, target);
			return;
		}

		chan->sendMessage(client->getNickname() + " PRIVMSG", args, target);
	} else {
		Client* targetClient = Server::getInstance().getClient(target);

		if (!targetClient) {
			sendError(client, ERR_NOSUCHNICK, _401, target);
			return;
		}

		try {
			targetClient->sendMessage(client->getNickname() + " PRIVMSG", args,
									  target);
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
