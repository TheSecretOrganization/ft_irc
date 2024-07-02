#pragma once

#include <map>
#include <string>
#include <vector>

const std::string USERLEN = "12";
const std::string CREATION_DATE = "06/07/2024";
const std::string SERVER_NAME = "webserv";
const std::string NETWORK_NAME = "CIRCé";
const std::string VERSION = "1.0";
const std::string HOSTNAME = "localhost";
const std::string UMODES = "ai";
const std::string CMODES = "itkolb";
const std::string CPMODES = "kolb";
const std::string CHANLIMIT = "4";
const std::string MOTD = "Have a nice day :)";
const std::string FORBIDDEN_WORDS = "orange,pear,onion";

class Configuration {
  private:
	std::map<std::string, std::string> config;
	std::vector<std::string> forbiddenWords;

  public:
	Configuration();
	~Configuration();

	const std::vector<std::string>& getForbiddenWords() const;
	std::string getValue(const std::string& key) const;
	void setPassword(const std::string& newPassword);
};
