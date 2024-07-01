#include "commands/PassCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"

#define ERR_PASSWORD "Authentification failed"

PassCommand::PassCommand() : Command("PASS", 1, 1) {}

PassCommand::~PassCommand() {}

void PassCommand::execute(Client* client, const std::string& args) {
	if (alreadyRegistred(client) || needMoreParams(client, split(args, ' ')) ||
		passwdMismatch(client, args)) {
		return;
	}
	client->setStatus(PASSWD_OK);
}
