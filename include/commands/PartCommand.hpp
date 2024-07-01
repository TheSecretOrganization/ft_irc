#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class PartCommand : public Command {
  public:
	PartCommand();
	~PartCommand();

	void execute(Client* client, const std::string& args);
};
