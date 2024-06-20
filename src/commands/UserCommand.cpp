#include "commands/UserCommand.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

UserCommand::UserCommand() {}
UserCommand::~UserCommand() {}

void UserCommand::execute(Client* client, std::string args) {
	if (!client->getRealname().empty()) {
		sendError(client, ERR_ALREADYREGISTRED, _462);
	}

	std::vector<std::string> splitArgs = split(args, ' ');
	if (splitArgs.size() < 4 || splitArgs[0].empty() || splitArgs[1].empty() ||
		splitArgs[2].empty()) {
		sendError(client, ERR_NEEDMOREPARAMS, _461, "USER");
	}

	size_t userlen = stringToSizeT(
		Server::getInstance().getConfiguration().getValue("userlen"));
	if (splitArgs[0].size() > userlen) {
		splitArgs[0] = splitArgs[0].substr(0, userlen);
	}

	for (size_t i = 4; i < splitArgs.size(); i++) {
		splitArgs[3] = splitArgs[3] + " " + splitArgs[i];
	}

	if (splitArgs[3].empty() || splitArgs[3][0] != ':') {
		sendError(client, ERR_NEEDMOREPARAMS, _461, "USER");
	}

	client->setUsername(splitArgs[0]);
	client->setHostname(splitArgs[1]);
	client->setServername(splitArgs[2]);
	client->setRealname(splitArgs[3]);
}
