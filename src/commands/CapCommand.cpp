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

static void rplWelcome(Client* client) {
	client->sendMessage(
		RPL_WELCOME,
		"Welcome to the " +
			Server::getInstance().getConfiguration().getValue("networkName") +
			" Network, " + client->getNickname() + "!" + client->getUsername() +
			"@" + client->getHostname(),
		client->getNickname());
}

static void rplYourHost(Client* client) {
	client->sendMessage(
		RPL_YOURHOST,
		"Your host is " +
			Server::getInstance().getConfiguration().getValue("serverName") +
			", running version " +
			Server::getInstance().getConfiguration().getValue("version"),
		client->getNickname());
}

static void rplCreated(Client* client) {
	client->sendMessage(
		RPL_CREATED,
		"This server was created " +
			Server::getInstance().getConfiguration().getValue("creationDate"),
		client->getNickname());
}

static void rplMyInfo(Client* client) {
	client->sendMessage(
		RPL_MYINFO,
		Server::getInstance().getConfiguration().getValue("serverName") + " " +
			Server::getInstance().getConfiguration().getValue("version") + " " +
			Server::getInstance().getConfiguration().getValue("umodes") + " " +
			Server::getInstance().getConfiguration().getValue("cmodes") + " " +
			Server::getInstance().getConfiguration().getValue("cpmodes"),
		client->getNickname());
}

static void rplISupport(Client* client) {
	client->sendMessage(
		RPL_ISUPPORT,
		"USERLEN=" +
			Server::getInstance().getConfiguration().getValue("userlen") +
			" CHANLIMIT = " +
			Server::getInstance().getConfiguration().getValue("chanlimit") +
			" :are supported by this server",
		client->getNickname());
}

void CapCommand::execute(Client* client, std::string args) {
	try {
		if (args == "LS") {
			client->sendMessage("CAP", "", "* LS");
		} else if (args == "END") {
			if (client->getStatus() != USER_OK) {
				return Server::getInstance()
					.getCommandRegistry()
					.getCommand("error")
					->execute(client, ERR_REGISTRATION);
			} else {
				client->setStatus(REGISTRED);
			}

			rplWelcome(client);
			rplYourHost(client);
			rplCreated(client);
			rplMyInfo(client);
			rplYourHost(client);
			rplISupport(client);
			LusersCommand().execute(client, "");
			MotdCommand().execute(client, "");
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
