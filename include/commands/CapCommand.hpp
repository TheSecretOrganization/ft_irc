#pragma once

#include "Command.hpp"

class CapCommand : public Command {

  public:
	CapCommand();
	~CapCommand();

	void execute(Client* client, std::string args);
};
