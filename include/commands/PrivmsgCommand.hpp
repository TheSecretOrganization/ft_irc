#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class PrivmsgCommand : public Command {
  public:
	PrivmsgCommand();
	~PrivmsgCommand();

	void execute(Client* client, const std::string& args);
};
