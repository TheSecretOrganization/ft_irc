#include "Configuration.hpp"
#include "Command.hpp"

#include <iostream>
#include <string>

Configuration::Configuration() {
	config["userlen"] = USERLEN;
	config["creationDate"] = CREATION_DATE;
	config["serverName"] = SERVER_NAME;
	config["networkName"] = NETWORK_NAME;
	config["hostname"] = HOSTNAME;
	config["version"] = VERSION;
	config["password"] = "";
	config["umodes"] = UMODES;
	config["cmodes"] = CMODES;
	config["cpmodes"] = CPMODES;
	config["chanlimit"] = CHANLIMIT;
	config["motd"] = MOTD;
	forbiddenWords = Command::split(FORBIDDEN_WORDS, ',');
}

Configuration::~Configuration() {}

std::string Configuration::getValue(const std::string& key) const {
	if (config.find(key) != config.end()) {
		return config.at(key);
	}
	return "";
}

void Configuration::setPassword(const std::string& password) {
	config["password"] = password;
}
