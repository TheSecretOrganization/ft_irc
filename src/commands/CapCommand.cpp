#include "commands/CapCommand.hpp"

#include "Client.hpp"
#include "Configuration.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"
#include "commands/LusersCommand.hpp"
#include "commands/MotdCommand.hpp"

#include <exception>
#include <iostream>
#include <string>

#define ERR_REGISTRATION "Error during registration"

CapCommand::CapCommand() : Command("CAP", 1, 1) {}

CapCommand::~CapCommand() {}

void CapCommand::rplWelcome(Client* client) const {
	client->sendMessage(
		Server::getInstance().getPrefix(), RPL_WELCOME, client->getNickname(),
		"Welcome to the " +
			Server::getInstance().getConfiguration().getValue("networkName") +
			" Network, " + client->getNickname() + "!" + client->getUsername() +
			"@" + client->getHostname());
}

void CapCommand::rplYourHost(Client* client) const {
	client->sendMessage(
		Server::getInstance().getPrefix(), RPL_YOURHOST, client->getNickname(),
		"Your host is " +
			Server::getInstance().getConfiguration().getValue("serverName") +
			", running version " +
			Server::getInstance().getConfiguration().getValue("version"));
}

void CapCommand::rplCreated(Client* client) const {
	client->sendMessage(
		Server::getInstance().getPrefix(), RPL_CREATED, client->getNickname(),
		"This server was created on " +
			Server::getInstance().getConfiguration().getValue("creationDate"));
}

void CapCommand::rplMyInfo(Client* client) const {
	client->sendMessage(
		Server::getInstance().getPrefix(), RPL_MYINFO,
		client->getNickname() + " " +
			Server::getInstance().getConfiguration().getValue("serverName") +
			" " + Server::getInstance().getConfiguration().getValue("version") +
			" " + Server::getInstance().getConfiguration().getValue("umodes") +
			" " + Server::getInstance().getConfiguration().getValue("cmodes") +
			" " + Server::getInstance().getConfiguration().getValue("cpmodes"));
}

void CapCommand::rplISupport(Client* client) const {
	client->sendMessage(
		Server::getInstance().getPrefix(), RPL_ISUPPORT,
		client->getNickname() + " USERLEN=" +
			Server::getInstance().getConfiguration().getValue("userlen") +
			" CHANLIMIT = " +
			Server::getInstance().getConfiguration().getValue("chanlimit") +
			" :are supported by this server");
}

void CapCommand::execute(Client* client, const std::string& args) {
	try {
		if (args == "LS") {
			client->sendMessage(Server::getInstance().getPrefix(), "CAP",
								"* LS");
		} else if (args == "END") {
			if (client->getStatus() != USER_OK) {
				return client->sendError("ERROR", "", "registration failed");
			} else {
				client->setStatus(REGISTRED);
			}

			rplWelcome(client);
			rplYourHost(client);
			rplCreated(client);
			rplMyInfo(client);
			rplISupport(client);
			LusersCommand().execute(client, "");
			MotdCommand().execute(client, "");
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
