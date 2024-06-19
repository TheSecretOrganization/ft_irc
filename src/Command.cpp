#include "Command.hpp"
#include "IrcReplies.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

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
bool	Command::needMoreParams(Client* client, std::vector<std::string>& vecArgs) {
	if (vecArgs.size() != expectedSize) {
		sendError(client, ERR_NEEDMOREPARAMS, _461, command);
		return	true;
	}
	return false;
}

bool	Command::noSuchChannel(Client* client, Channel* channel, std::string channelName) {
	if (channel == NULL) {
		sendError(client, ERR_NOSUCHCHANNEL, _403, channelName);
		return true;
	}
	return false;
}

bool	Command::notOnChannel(Client* client, Channel* channel) {
	if (!channel->isUserOnChannel(client)) {
		sendError(client, ERR_NOTONCHANNEL, _442, channel->getChannelName());
		return true;
	}
	return false;
}

bool	Command::userOnChannel(Client* client, Channel* channel, std::string nick) {
	if (channel->isUserOnChannel(Server::getInstance().getClient(nick))) {
		sendError(client, ERR_USERONCHANNEL, _443, nick);
		return true;
	}
	return false;
}

std::vector<std::string> Command::split(const std::string& str,
										char del) const {
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
