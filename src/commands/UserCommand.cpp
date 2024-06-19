#include "commands/UserCommand.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

UserCommand::UserCommand() {}
UserCommand::~UserCommand() {}

void UserCommand::execute(Client* client, std::string args) {
	if (client->getRealname().empty()) {
		throw AlreadyRegisterException(client);
	}

	std::vector<std::string> splitArgs = split(args, ' ');
	if (splitArgs.size() < 4 || splitArgs[1] != "0" || splitArgs[2] != "*" ||
		splitArgs[0].empty()) {
		throw NeedMoreParamsException(client, USER);
	}

	if (splitArgs[0].size() > Server::getInstance().getConfiguration().getUserlen()) {
		splitArgs[0] = splitArgs[0].substr(0, Server::getInstance().getConfiguration().getUserlen());
	}

	for (size_t i = 4; i < splitArgs.size(); i++) {
		splitArgs[3] = splitArgs[3] + " " + splitArgs[i];
	}

	if (splitArgs[3].empty() || splitArgs[3][0] != ':') {
		throw NeedMoreParamsException(client, USER);
	}

	client->setUsername(splitArgs[0]);
	client->setRealname(splitArgs[3]);
}

UserCommand::AlreadyRegisterException::AlreadyRegisterException(
	const Client* client)
	: client(client) {}

const char* UserCommand::AlreadyRegisterException::what() const throw() {
	if (client == NULL)
		return ERR_ALREADYREGISTRED;
	try {
		client->sendMessage(ERR_ALREADYREGISTRED,
							client->getNickname() + " :You may not reregister");
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return ERR_ALREADYREGISTRED;
}
