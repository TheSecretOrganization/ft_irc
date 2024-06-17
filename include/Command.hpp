#pragma once

#include "Client.hpp"

class Command {

public:
	virtual ~Command() {};

	virtual void execute(Client *client, std::string args) = 0;

};
