#include "commands/PingCommand.hpp"

#include <iostream>

PingCommand::PingCommand() {}
PingCommand::~PingCommand() {}

void PingCommand::execute(Client *client, std::string args){
	std::string arg = args.substr(6, args.size() - 1);
	std::cout << "Got ping from " << client->getSocket().getFd()
		<< " with " << arg << std::endl;
	client->sendMessage("PONG", arg);
}
