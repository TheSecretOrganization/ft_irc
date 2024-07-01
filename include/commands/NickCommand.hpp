#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include <string>

class NickCommand : public Command {

  public:
	NickCommand();
	~NickCommand();

	void execute(Client* client, const std::string& args);
};
