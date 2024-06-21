#include "commands/PassCommand.hpp"
#include "commands/ErrorCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"

#define ERR_PASSWORD "Authentification failed"

PassCommand::PassCommand() : Command("PASS", 1, 1) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client* client, std::string args) {
	if (alreadyRegistred(client) || needMoreParams(client, split(args, ' '))) {
		return;
	}

	if (passwdMismatch(client, args)) {
		return ErrorCommand::executeError(client, "");
	}
    client->setStatus(PASSWD_OK);
}
