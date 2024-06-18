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
	if (args.empty())
		throw NoNicknameGivenException(client);
	if (checkNicknameValid(args))
		throw ErrOneUsNicknameException(client, &args);
	if (checkAlreadyInUse(args) == false)
		throw NicknameInUseException(client, &args);
	client->setNickname(args);
}

NickCommand::NoNicknameGivenException::NoNicknameGivenException(Client* client)
	: client(client) {}

const char* NickCommand::NoNicknameGivenException::what() const throw() {
	if (client == NULL)
		return ERR_NONICKNAMEGIVEN;
	try {
		client->sendMessage(ERR_NONICKNAMEGIVEN,
							client->getNickname() + " :No nickname given");
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return ERR_NONICKNAMEGIVEN;
}

NickCommand::ErrOneUsNicknameException::ErrOneUsNicknameException(
	Client* client, const std::string* args)
	: client(client), args(args) {}

const char* NickCommand::ErrOneUsNicknameException::what() const throw() {
	if (client == NULL || args == NULL)
		return ERR_ERRONEUSNICKNAME;
	try {
		client->sendMessage(ERR_ERRONEUSNICKNAME, client->getNickname() + " " +
													  *args +
													  " :Erroneus nickname");
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return ERR_ERRONEUSNICKNAME;
}

NickCommand::NicknameInUseException::NicknameInUseException(
	Client* client, const std::string* args)
	: client(client), args(args) {}

const char* NickCommand::NicknameInUseException::what() const throw() {
	if (client == NULL || args == NULL)
		return ERR_NICKNAMEINUSE;
	try {
		client->sendMessage(ERR_NICKNAMEINUSE,
							client->getNickname() + " " + *args +
								" :Nickname is already in use");
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return ERR_NICKNAMEINUSE;
}
