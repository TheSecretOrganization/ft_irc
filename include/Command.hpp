#pragma once

#include "Client.hpp"

class Command {

public:
	virtual void execute(Client &client) = 0;

};
