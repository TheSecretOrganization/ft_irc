#pragma once

#include "Client.hpp"

#include <string>

#define BOT_SOCKET -2

class Bot : public Client {
  public:
	Bot();
	~Bot();

	Client& getBot();
	const std::string getNameFromPrefix(const std::string& prefix);
	void kickUser(const std::string& prefix, const std::string& channel, const std::string& word);
	void scanMessage(const std::string& prefix, const std::string& channel, const std::string& message);
};
