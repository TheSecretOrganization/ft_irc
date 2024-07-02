#include "Bot.hpp"
#include "Client.hpp"
#include "Server.hpp"

#include <iostream>
#include <string>
#include <vector>

Bot::Bot() : Client(BOT_SOCKET) {
	nickname = "Guardian";
	realname = "GuardBot";
	username = "Bot";
	bot = true;
	status = REGISTRED;
}

Bot::~Bot() {}

Client* Bot::getClientFromPrefix(const std::string& prefix) const {
	return Server::getInstance().getClient(prefix.substr(0, prefix.find("!")));
}

bool Bot::isMessageAuthorized(const std::string& prefix, Channel* channel,
							  const std::string& message) {
	const std::vector<std::string>& forbiddenWords =
		Server::getInstance().getConfiguration().getForbiddenWords();

	std::cout << "Guardian is checking: " << message << std::endl;

	for (std::vector<std::string>::const_iterator it = forbiddenWords.begin();
		 it != forbiddenWords.end(); it++) {
		if (message.find((*it), 0) != std::string::npos) {
			channel->kick(this, getClientFromPrefix(prefix),
						  "Use of forbidden word: " + *it);
			return false;
		}
	}
	return true;
}
