#include "Command.hpp"
#include "IrcReplies.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> Command::split(const std::string& str, char del) const {
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

Command::NeedMoreParamsException::NeedMoreParamsException(
	const Client* client, const std::string* args)
	: client(client), args(args) {}

const char* Command::NeedMoreParamsException::what() const throw() {
    if (client == NULL || args == NULL)
        	return ERR_NEEDMOREPARAMS;
	try {
		client->sendMessage(ERR_NEEDMOREPARAMS,
							client->getNickname() + " " + *args + " :Not enough parametersr");
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return ERR_NEEDMOREPARAMS;
}
