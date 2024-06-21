#include "commands/PingCommand.hpp"

#include <iostream>

PingCommand::PingCommand() {}
PingCommand::~PingCommand() {}

void PingCommand::execute(Client* client, std::string args) {
	try {
		client->sendMessage("PONG", args);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
