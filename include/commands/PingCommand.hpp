#pragma once

#include "Command.hpp"

class PingCommand : public Command {

  public:
	PingCommand();
	~PingCommand();

	void execute(Client* client, std::string args);
};
