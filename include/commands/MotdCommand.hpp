#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class MotdCommand : public Command {

  public:
	MotdCommand();
	~MotdCommand();

	void execute(Client* client, const std::string& args);
};
