#pragma once

#include "Client.hpp"
#include <string>
#include <vector>

class Command {

  public:
	virtual ~Command(){};

	virtual void execute(Client* client, std::string args) = 0;
	std::vector<std::string> split(const std::string& str, char del) const;
};
