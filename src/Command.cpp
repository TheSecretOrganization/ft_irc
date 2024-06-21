#include "Command.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

Command::Command(const std::string& name, size_t expectedSize, size_t minSize)
	: name(name), expectedSize(expectedSize), minSize(minSize) {}

Command::~Command() {}

void Command::sendError(Client* client, std::string code, std::string message,
						std::string arg) const {
	try {
		if (!arg.empty())
			client->sendMessage(code, arg + " :" + message);
		else
			client->sendMessage(code, ":" + message);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

std::vector<std::string> Command::split(const std::string& str, char del) {
	std::vector<std::string> result;
	std::string tmp;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (*it != del) {
			tmp += *it;
		} else if (!tmp.empty()) {
			result.push_back(tmp);
			tmp.clear();
		}
	}
	if (!tmp.empty()) {
		result.push_back(tmp);
	}
	return result;
}

bool Command::needMoreParams(Client* client,
							 const std::vector<std::string>& vecArgs) const {
	if (expectedSize != 0 && vecArgs.size() != expectedSize) {
		sendError(client, ERR_NEEDMOREPARAMS, _461, name);
		return true;
	} else if (vecArgs.size() < minSize) {
		sendError(client, ERR_NEEDMOREPARAMS, _461, name);
		return true;
	}
	return false;
}

bool Command::alreadyRegistred(Client* client) const {
	if (client->getStatus() == REGISTRED) {
		sendError(client, ERR_ALREADYREGISTRED, _462);
		return true;
	}
	return false;
}

bool Command::passwdMismatch(Client* client, const std::string& passWd) const {
	if (passWd != Server::getInstance().getConfiguration().getValue("password")) {
		sendError(client, ERR_PASSWDMISMATCH, _464);
		return true;
	}
	return false;
}
