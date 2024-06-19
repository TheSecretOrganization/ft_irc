#pragma once

#include "Client.hpp"
#include <string>

class Command {

  public:
	virtual ~Command(){};

	virtual void execute(Client* client, std::string args) = 0;
	void sendError(Client* client, std::string code, std::string meassage,
				   std::string arg = "") const;
};
