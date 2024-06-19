#pragma once

#include <string>

#include "ClientSocket.hpp"

class Client {

  private:
	ClientSocket socket;
	std::string realname;
	std::string username;
	std::string nickname;
	std::string	hostname;
	std::string servername;

  public:
	Client(int fd);
	~Client();

	ClientSocket& getSocket();

	void sendMessage(std::string type, std::string message) const;
	std::string getNickname() const;
	void setNickname(const std::string& newNickname);
	std::string getUsername() const;
	void setUsername(const std::string& newUsername);
	std::string getRealname() const;
	void setRealname(const std::string& newRealname);
	std::string getHostname() const;
	void setHostname(const std::string& newHostname);
	std::string getServername() const;
	void setServername(const std::string& newServername);
};
