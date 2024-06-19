#pragma once

#include <cstddef>
#include <string>

#define USERLEN 12
#define CREATION_DATE "06/07/2024"
#define SERVER_NAME "webserv"
#define NETWORK_NAME "CIRCé"
#define VERSION "1.0"
#define HOSTNAME "localhost"

class Configuration {
  private:
	size_t userlen;
	std::string creationDate;
	std::string serverName;
	std::string networkName;
	std::string hostname;
	std::string password;
	std::string version;

  public:
	Configuration();
	~Configuration();

	size_t getUserlen() const;
	const std::string& getCreationDate() const;
	const std::string& getServerName() const;
	const std::string& getNetworkName() const;
	const std::string& getHostname() const;
	const std::string& getPassword() const;
	void setPassword(const std::string& newPassword);
	const std::string& getVersion() const;
};
