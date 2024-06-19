#include "commands/PingCommand.hpp"

#include <iostream>

PingCommand::PingCommand() {}
PingCommand::~PingCommand() {}

void PingCommand::execute(Client* client, std::string args) {
	std::cout << "Got ping from " << client->getSocket().getFd() << " with "
			  << args << std::endl;
	try {
		client->sendMessage("PONG", args);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
