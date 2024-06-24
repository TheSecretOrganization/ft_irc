#include "commands/ErrorCommand.hpp"
#include "Server.hpp"

#include <exception>
#include <iostream>

#define ERROR "ERROR"
#define DEFAULT_ERR_MSG "Unknown server error"

ErrorCommand::ErrorCommand() : Command(ERROR, 1, 1) {}

ErrorCommand::~ErrorCommand() {}

void ErrorCommand::execute(Client* client, std::string args) {
	if (args.empty())
		args = DEFAULT_ERR_MSG;
	sendError(client, ERROR, args);
	try {
		Server::getInstance().deleteClient(client);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
