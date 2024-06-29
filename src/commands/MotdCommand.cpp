#include "commands/MotdCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

MotdCommand::MotdCommand() : Command("MOTD", 0, 0) {}

MotdCommand::~MotdCommand() {}

void MotdCommand::execute(Client* client, const std::string& args) {
	if (noSuchServer(client, args))
		return;
	client->sendMessage(
		Server::getInstance().getPrefix(), RPL_MOTDSTART, client->getNickname(),
		"- " + Server::getInstance().getConfiguration().getValue("servername") +
			" Message of the day -");
	client->sendMessage(
		Server::getInstance().getPrefix(), RPL_MOTD, client->getNickname(),
		Server::getInstance().getConfiguration().getValue("motd"));
	client->sendMessage(Server::getInstance().getPrefix(), RPL_ENDOFMOTD,
						client->getNickname(), "End of /MOTD command.");
}
