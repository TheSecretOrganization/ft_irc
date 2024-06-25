#include "ServerCommands.hpp"
#include "commands/ErrorCommand.hpp"

ServerCommands::ServerCommands() {
	registerCommand("error", new ErrorCommand());
}

ServerCommands::~ServerCommands() {}
