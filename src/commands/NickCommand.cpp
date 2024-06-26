#include "commands/NickCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cctype>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#define VALID_CHARS "{}[]|\\"

NickCommand::NickCommand() : Command("NICK", 1, 1) {}

NickCommand::~NickCommand() {}

static bool checkNicknameValid(const std::string& nickname) {
	for (size_t i = 0; nickname[i]; i++) {
		if (!isalnum(nickname[i]) && !strchr(VALID_CHARS, nickname[i])) {
			return false;
		}
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

void NickCommand::execute(Client* client, const std::string& args) {
	if (args.empty())
		return client->sendError(ERR_NONICKNAMEGIVEN,
								 client->getClientnickName(), _431);
	if (checkNicknameValid(args) == false)
		return client->sendError(ERR_ERRONEUSNICKNAME,
								 client->getClientnickName() + " " + args,
								 _432);
	if (checkAlreadyInUse(args) == false)
		return client->sendError(
			ERR_NICKNAMEINUSE, client->getClientnickName() + " " + args, _433);
	client->setNickname(args);
	if (client->getStatus() == PASSWD_OK)
		client->setStatus(NICK_OK);
}
