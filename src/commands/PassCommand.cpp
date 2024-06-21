#include "commands/PassCommand.hpp"
#include "Command.hpp"

PassCommand::PassCommand() : Command("PASS", 1, 1) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client* client, std::string args) {

}
