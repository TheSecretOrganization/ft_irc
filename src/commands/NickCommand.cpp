#include "commands/NickCommand.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cctype>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#define VALID_CHARS "{}[]|\\"

NickCommand::NickCommand() {}
NickCommand::~NickCommand() {}

static bool checkNicknameValid(const std::string& nickname) {
	for (size_t i = 0; nickname[i]; i++) {
		if (!isalnum(nickname[i]) && !strchr(VALID_CHARS, nickname[i]))
			return false;
	}
	return true;
}

static bool checkAlreadyInUse(const std::string& nickname) {
	for (size_t i = 0; i < Server::getInstance().getClients().size(); i++) {
		if (Server::getInstance().getClients()[i]->getNickname() == nickname) {
			return false;
		}
	}
	return true;
}

void NickCommand::execute(Client* client, std::string args) {
	try {
		if (args.empty()) {
			client->sendMessage(ERR_NONICKNAMEGIVEN,
								client->getNickname() + " :No nickname given");
		} else if (checkNicknameValid(args)) {
			client->sendMessage(ERR_ERRONEUSNICKNAME,
								client->getNickname() + " " + args +
									" :Erroneus nickname");
		} else if (checkAlreadyInUse(args) == false) {
			client->sendMessage(ERR_NICKNAMEINUSE,
								client->getNickname() + " " + args +
									" :Nickname is already in use");
		} else {
			client->setNickname(args);
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
