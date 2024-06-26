#include "commands/MotdCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

MotdCommand::MotdCommand() : Command("MOTD", 0, 0) {}

MotdCommand::~MotdCommand() {}

void MotdCommand::execute(Client* client, std::string args) {
	if (noSuchServer(client, args))
		return;
	client->sendMessage(
		RPL_MOTDSTART,
		"- " + Server::getInstance().getConfiguration().getValue("hostname") +
			" Message of the day -",
		client->getNickname());
	client->sendMessage(
		RPL_MOTD, Server::getInstance().getConfiguration().getValue("motd"),
		client->getNickname());
	client->sendMessage(RPL_ENDOFMOTD, "End of /MOTD command.",
						client->getNickname());
}
