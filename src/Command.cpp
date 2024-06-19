#include "Command.hpp"
#include "IrcReplies.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

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

Command::NeedMoreParamsException ::NeedMoreParamsException(const Client* client,
														   int command)
	: client(client), command(command) {}

const char* Command::NeedMoreParamsException::what() const throw() {
	if (client == NULL)
		return ERR_NEEDMOREPARAMS;
	try {
		switch (command) {
		case USER:
			client->sendMessage(ERR_NEEDMOREPARAMS,
								client->getNickname() +
									" USER :Not enough parametersr");
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return ERR_NEEDMOREPARAMS;
}
