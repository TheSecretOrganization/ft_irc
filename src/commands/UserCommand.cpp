#include "commands/UserCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

UserCommand::UserCommand() : Command("USER", 0, 4) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client* client, const std::string& args) {
	if (alreadyRegistred(client)) {
		return;
	}

	std::vector<std::string> splitArgs = split(args, ' ');
	if (needMoreParams(client, splitArgs)) {
		return;
	}

	size_t userlen = std::atoi(
		Server::getInstance().getConfiguration().getValue("userlen").c_str());
	if (splitArgs[0].size() > userlen) {
		splitArgs[0] = splitArgs[0].substr(0, userlen);
	}

	for (size_t i = 4; i < splitArgs.size(); i++) {
		splitArgs[3] = splitArgs[3] + " " + splitArgs[i];
	}

	if (splitArgs[3][0] != ':') {
		client->sendError(ERR_NEEDMOREPARAMS,
						  client->getClientnickName() + " " + name, _461);
		return;
	}

	client->setUsername(splitArgs[0]);
	client->setHostname(splitArgs[1]);
	client->setServername(splitArgs[2]);
	client->setRealname(splitArgs[3]);
	if (client->getStatus() == NICK_OK)
		client->setStatus(USER_OK);
}
