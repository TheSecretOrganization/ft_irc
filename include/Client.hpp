#pragma once

#include <string>

#include "ClientSocket.hpp"

class Client {

  private:
	ClientSocket socket;
	std::string name;
	std::string nickname;

  public:
	Client(int fd);
	~Client();

	ClientSocket& getSocket();

	void sendMessage(std::string type, std::string message) const;
	std::string getNickname() const;
	void setNickname(const std::string& newNickname);
};
