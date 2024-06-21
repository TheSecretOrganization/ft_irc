#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include <string>

class ErrorCommand : public Command {
  public:
	ErrorCommand();
	~ErrorCommand();

	void execute(Client* client, std::string args);
};
