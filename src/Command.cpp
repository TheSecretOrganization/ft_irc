#include "Command.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <sstream>
#include <string>
#include <vector>

Command::Command(const std::string& name, size_t expectedSize, size_t minSize)
	: name(name), expectedSize(expectedSize), minSize(minSize) {}

Command::~Command() {}

bool Command::needMoreParams(Client* client,
							 const std::vector<std::string>& vecArgs) const {
	if ((expectedSize != 0 && vecArgs.size() != expectedSize) ||
		(vecArgs.size() < minSize)) {
		client->sendError(ERR_NEEDMOREPARAMS,
						  client->getClientnickName() + " " + name, _461);
		return true;
	}
	return false;
}

bool Command::noSuchChannel(Client* client, Channel* channel,
							std::string channelName) const {
	if (channel == NULL) {
		client->sendError(ERR_NOSUCHCHANNEL,
						  client->getClientnickName() + " " + channelName,
						  _403);
		return true;
	}
	return false;
}

bool Command::notOnChannel(Client* client, Channel* channel) const {
	if (!channel->isUserOnChannel(client)) {
		client->sendError(
			ERR_NOTONCHANNEL,
			client->getClientnickName() + " " + channel->getName(), _442);
		return true;
	}
	return false;
}

bool Command::userOnChannel(Client* client, Channel* channel,
							std::string nick) const {
	if (channel->isUserOnChannel(Server::getInstance().getClient(nick))) {
		client->sendError(ERR_USERONCHANNEL,
						  client->getClientnickName() + " " + nick + " " +
							  channel->getName(),
						  _443);
		return true;
	}
	return false;
}

bool Command::alreadyRegistred(Client* client) const {
	if (client->getStatus() == REGISTRED) {
		client->sendError(ERR_ALREADYREGISTRED, client->getClientnickName(),
						  _462);
		return true;
	}
	return false;
}

bool Command::passwdMismatch(Client* client, const std::string& passWd) const {
	if (passWd !=
		Server::getInstance().getConfiguration().getValue("password")) {
		client->sendError(ERR_PASSWDMISMATCH, _464);
		return true;
	}
	return false;
}

bool Command::noSuchServer(Client* client, const std::string& server) const {
	if (!server.empty() &&
		server !=
			Server::getInstance().getConfiguration().getValue("servername")) {
		client->sendError(ERR_NOSUCHSERVER, server, _402);
		return true;
	}
	return false;
}

bool Command::chanOPrivsNeeded(Client* client, Channel* channel) const {
	if (channel->isInviteMode() && !channel->isUserOperator(client)) {
		client->sendError(ERR_CHANOPRIVSNEEDED, channel->getName(), _482);
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

std::string Command::trim(const std::string& str) {
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

std::string Command::size_tToString(size_t value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}
