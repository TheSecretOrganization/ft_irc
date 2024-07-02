#pragma once

#include <map>
#include <string>
#include <vector>

#define USERLEN "12"
#define CREATION_DATE "06/07/2024"
#define SERVER_NAME "webserv"
#define NETWORK_NAME "CIRCé"
#define VERSION "1.0"
#define HOSTNAME "localhost"
#define UMODES "ai"
#define CMODES "itkolb"
#define CPMODES "kolb"
#define CHANLIMIT "4"
#define MOTD "Have a nice day :)"
#define FORBIDDEN_WORDS "orange,pear,onion"

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
