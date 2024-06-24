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

bool Command::noSuchChannel(Client* client, Channel* channel,
							std::string channelName) const {
	if (channel == NULL) {
		sendError(client, ERR_NOSUCHCHANNEL, _403, channelName);
		return true;
	}
	return false;
}

bool Command::notOnChannel(Client* client, Channel* channel) const {
	if (!channel->isUserOnChannel(client)) {
		sendError(client, ERR_NOTONCHANNEL, _442, channel->getChannelName());
		return true;
	}
	return false;
}

bool Command::userOnChannel(Client* client, Channel* channel,
							std::string nick) const {
	if (channel->isUserOnChannel(Server::getInstance().getClient(nick))) {
		sendError(client, ERR_USERONCHANNEL, _443, nick);
		return true;
	}
	return false;
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

bool Command::alreadyRegistred(Client* client) const {
	if (client->getStatus() == REGISTRED) {
		sendError(client, ERR_ALREADYREGISTRED, _462);
		return true;
	}
	return false;
}

bool Command::passwdMismatch(Client* client, const std::string& passWd) const {
	if (passWd !=
		Server::getInstance().getConfiguration().getValue("password")) {
		sendError(client, ERR_PASSWDMISMATCH, _464);
		return true;
	}
	return false;
}

bool Command::noSuchServer(Client* client, const std::string& server) const {
	if (!server.empty() &&
		server !=
			Server::getInstance().getConfiguration().getValue("hostname")) {
		sendError(client, ERR_NOSUCHSERVER, _402, server);
		return true;
	}
	return false;
}
