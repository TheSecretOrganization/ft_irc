#include "commands/PassCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"

PassCommand::PassCommand() : Command("PASS", 1, 1) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client* client, std::string args) {
	if (alreadyRegistred(client) || needMoreParams(client, split(args, ' '))) {
		return;
	}

	if (passwdMismatch(client, args)) {
        // TODO: Error
		return;
	}
    client->setStatus(PASSWD_OK);
}
