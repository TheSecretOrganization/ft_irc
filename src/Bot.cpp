#include "Bot.hpp"
#include "Server.hpp"

#include <iostream>
#include <vector>
#include <iterator>

Bot::Bot() {}

Bot::~Bot() {}

const std::string& Bot::getNameFromPrefix(const std::string& prefix) {
	return prefix.substr(0, prefix.find("!"));
}

void Bot::kickUser(const std::string& prefix, const std::string& channel, const std::string& word) {
	Server::getInstance().getCommandRegistry().getCommand("kick")->execute(bot, channel + " " + getNameFromPrefix(prefix) + " :Use of forbidden word: " + word);
}

void Bot::scanMessage(const std::string& prefix, const std::string& channel, const std::string& message) {
	const std::vector<std::string>& forbiddenWords = Server::getInstance().getConfiguration().getForbiddenWords();
	for (std::vector<std::string>::const_iterator it = forbiddenWords.begin(); it != forbiddenWords.end(); it++)
	{
		if (message.find((*it), 0) == message.npos) {
			kickUser(prefix, channel,(*it));
			break;
		}
	}
}
