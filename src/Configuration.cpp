#include "Configuration.hpp"

#include <cstddef>

Configuration::Configuration()
	: userlen(USERLEN), creationDate(CREATION_DATE), serverName(SERVER_NAME),
	  networkName(NETWORK_NAME), hostname(HOSTNAME), password(""),
	  version(VERSION) {}

Configuration::~Configuration() {}

size_t Configuration::getUserlen() const { return userlen; }

const std::string& Configuration::getCreationDate() const {
	return creationDate;
}

const std::string& Configuration::getServerName() const { return serverName; }

const std::string& Configuration::getNetworkName() const { return networkName; }

const std::string& Configuration::getHostname() const { return hostname; }

const std::string& Configuration::getPassword() const { return password; }

void Configuration::setPassword(const std::string& newPassword) {
	password = newPassword;
}

const std::string& Configuration::getVersion() const { return version; }
