#pragma once

#include "Client.hpp"

#include <string>

const int BOT_SOCKET = -2;

class Bot : public Client {
  private:
	Client* getClientFromPrefix(const std::string& prefix) const;

  public:
	Bot();
	~Bot();

	bool isMessageAuthorized(const std::string& prefix, Channel* channel,
							 const std::string& message);
};
