#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class KickCommand : public Command {
  public:
	KickCommand();
	~KickCommand();

	void execute(Client* client, const std::string& args);
};
