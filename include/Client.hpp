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
	std::string getClientnickName(void);

	void sendMessage(std::string type, std::string message) const;
};
