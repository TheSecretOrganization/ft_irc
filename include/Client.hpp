#pragma once

#include <string>

#include "ClientSocket.hpp"

class Client {

  private:
	ClientSocket socket;
	std::string name;

  public:
	Client(int fd);
	~Client();

	ClientSocket& getSocket();

	void sendMessage(std::string type, std::string message) const;
};
