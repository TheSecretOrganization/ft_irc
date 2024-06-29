#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include <string>

class WhoCommand : public Command {

  public:
	WhoCommand();
	~WhoCommand();

	void execute(Client* client, const std::string& args);
};
