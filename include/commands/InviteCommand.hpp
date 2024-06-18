#pragma once

#include "Command.hpp"

class InviteCommand : public Command {

  public:
	InviteCommand();
	~InviteCommand();

	void execute(Client* client, std::string args);
};
