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
		client->getNickname() + " :- " +
			Server::getInstance().getConfiguration().getValue("hostname") +
			" Message of the day -");
	client->sendMessage(
		RPL_MOTD,
		client->getNickname() + " :" +
			Server::getInstance().getConfiguration().getValue("motd"));
	client->sendMessage(RPL_ENDOFMOTD,
						client->getNickname() + " :End of /MOTD command.");
}
