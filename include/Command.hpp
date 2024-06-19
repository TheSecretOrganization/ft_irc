#pragma once

#include "Client.hpp"
#include <string>
#include <vector>

class Command {

  public:
	virtual ~Command(){};

	virtual void execute(Client* client, std::string args) = 0;
	void sendError(Client* client, std::string code, std::string meassage,
				   std::string arg = "") const;
	std::vector<std::string> split(const std::string& str, char del) const;
};
