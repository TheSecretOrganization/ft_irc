#pragma once

#include "Client.hpp"

class Command {

public:
	virtual ~Command();

	virtual void execute(Client &client) = 0;

};
