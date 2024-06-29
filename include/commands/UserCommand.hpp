#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include <string>

class UserCommand : public Command {

  public:
	UserCommand();
	~UserCommand();

	void execute(Client* client, const std::string& args);
};
