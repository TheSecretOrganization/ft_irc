#include "commands/PingCommand.hpp"
#include "Command.hpp"

#include <iostream>

PingCommand::PingCommand() : Command("PING", 1, 1) {}

PingCommand::~PingCommand() {}

void PingCommand::execute(Client* client, std::string args) {
	try {
		client->sendMessage("PONG", args);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
