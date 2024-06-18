#pragma once

#include "Command.hpp"

class NickCommand : public Command {

  public:
	NickCommand();
	~NickCommand();

	void execute(Client* client, std::string args);
};
