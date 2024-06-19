#include "commands/CapCommand.hpp"
#include "Client.hpp"
#include "Configuration.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <exception>
#include <iostream>
#include <string>

CapCommand::CapCommand() {}
CapCommand::~CapCommand() {}

static void rplWelcome(Client* client) {
	client->sendMessage(
		RPL_WELCOME,
		client->getNickname() + " :Welcome to the " +
			Server::getInstance().getConfiguration().getValue("networkName") +
			"Network, " + client->getNickname() + "[" + "@" + "]");
}

static void rplYourHost(Client* client) {
	client->sendMessage(
		RPL_YOURHOST,
		client->getNickname() + " :Your host is " +
			Server::getInstance().getConfiguration().getValue("serverName") +
			", running version " +
			Server::getInstance().getConfiguration().getValue("version"));
}

static void rplCreated(Client* client) {
	client->sendMessage(
		RPL_CREATED,
		client->getNickname() + " :This server was created " +
			Server::getInstance().getConfiguration().getValue("creationDate"));
}

static void rplMyInfo(Client* client) {
	client->sendMessage(
		RPL_MYINFO,
		client->getNickname() + " " +
			Server::getInstance().getConfiguration().getValue("serverName") +
			" " + Server::getInstance().getConfiguration().getValue("version") +
			" " + Server::getInstance().getConfiguration().getValue("umodes") +
			" " + Server::getInstance().getConfiguration().getValue("cmodes") +
			" " + Server::getInstance().getConfiguration().getValue("cpmodes"));
}

static void rplISupport(Client* client) {
	client->sendMessage(
		RPL_ISUPPORT,
		client->getNickname() + " USERLEN=" +
			Server::getInstance().getConfiguration().getValue("userlen") +
			" :are supported by this server");
}

void CapCommand::execute(Client* client, std::string args) {
	try {
		if (args == "LS") {
			client->sendMessage("CAP * LS", ":");
		} else if (args == "END") {
			rplWelcome(client);
			rplYourHost(client);
			rplCreated(client);
			rplMyInfo(client);
			rplYourHost(client);
			rplISupport(client);
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
