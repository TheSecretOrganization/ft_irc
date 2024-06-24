#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class QuitCommand : public Command {

  public:
	QuitCommand();
	~QuitCommand();

	void execute(Client* client, std::string args);
};
