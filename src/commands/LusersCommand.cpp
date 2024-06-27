#include "commands/LusersCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <sstream>
#include <string>

LusersCommand::LusersCommand() : Command("LUSERS", 0, 0) {}

LusersCommand::~LusersCommand() {}

static std::string size_tToString(size_t value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

void LusersCommand::rplLuserClient(Client* client) const {
	std::string nbClients =
		size_tToString(Server::getInstance().getClients().size());
	client->sendMessage(Server::getInstance().getPrefix(), RPL_LUSERCLIENT,
						client->getNickname(),
						"There are " + nbClients +
							" users and 0 invisible on 1 servers");
}

void LusersCommand::rplLuserMe(Client* client) const {
	std::string nbClients =
		size_tToString(Server::getInstance().getClients().size());
	client->sendMessage(Server::getInstance().getPrefix(), RPL_LUSERME,
						client->getNickname(),
						"I have " + nbClients + " users and 1 servers");
}

void LusersCommand::execute(Client* client, std::string args) {
	(void)args;
	rplLuserClient(client);
	rplLuserMe(client);
}
