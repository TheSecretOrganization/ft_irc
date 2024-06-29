#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class TopicCommand : public Command {
  public:
	TopicCommand();
	~TopicCommand();

	void execute(Client* client, std::string args);
};
