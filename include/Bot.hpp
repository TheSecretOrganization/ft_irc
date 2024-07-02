#pragma once

#include "Client.hpp"

#include <string>

const int BOT_SOCKET = -2;

class Bot : public Client {
  private:
	std::string getNameFromPrefix(const std::string& prefix) const;

  public:
	Bot();
	~Bot();

	bool isMessageAuthorized(const std::string& prefix,
							 const std::string& channel,
							 const std::string& message);
};
