#include "commands/QuitCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"

#include <string>
#include <vector>

QuitCommand::QuitCommand() : Command("QUIT", 0, 0) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client* client, const std::string& args) {
	std::vector<Channel*> channels = Server::getInstance().getChannels(client);

	for (std::vector<Channel*>::iterator it = channels.begin();
		 it != channels.end(); it++) {
		(*it)->removeUser(client);
		(*it)->broadcast(client->getPrefix(), "QUIT",
						 args.empty() ? "has been absorbed by the Black Hole"
									  : args);
	}

	client->sendError("ERROR", "", "Bye for now!");
}
