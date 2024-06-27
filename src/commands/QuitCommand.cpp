#include "commands/QuitCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"

#include <string>
#include <vector>

QuitCommand::QuitCommand() : Command("QUIT", 0, 0) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client* client, std::string args) {
	if (args.empty())
		args = "left the server";

	std::string message = "Quit: " + client->getNickname() + " " + args;
	std::vector<Channel*> channels = Server::getInstance().getChannels(client);

	for (std::vector<Channel*>::iterator it = channels.begin();
		 it != channels.end(); it++) {
		(*it)->removeUser(client);
		(*it)->sendMessage(message);
	}

	client->sendError("ERROR", "Quit: Bye for now!");
}
