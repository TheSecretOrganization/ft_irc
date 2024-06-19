#include "commands/NickCommand.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cctype>
#include <cstddef>
#include <cstring>
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
	if (args.empty())
		return sendError(client, ERR_NONICKNAMEGIVEN, _431);
	if (checkNicknameValid(args))
		return sendError(client, ERR_ERRONEUSNICKNAME, _432, args);
	if (checkAlreadyInUse(args) == false)
		return sendError(client, ERR_NICKNAMEINUSE, _433, args);
	client->setNickname(args);
}
