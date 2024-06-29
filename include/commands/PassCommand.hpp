#pragma once

#include "Command.hpp"

class PassCommand : public Command {

  public:
	PassCommand();
	~PassCommand();

	void execute(Client* client, const std::string& args);
};
