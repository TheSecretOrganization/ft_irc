#include "commands/NickCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#define VALID_CHARS "{}[]|\\"

NickCommand::NickCommand() : Command("NICK", 1, 1) {}

NickCommand::~NickCommand() {}

static bool checkNicknameValid(const std::string& nickname) {
	for (size_t i = 0; nickname[i]; i++) {
		if (!isalnum(nickname[i]) && !strchr(VALID_CHARS, nickname[i])) {
			return false;
		}
	}
	return true;
}

static bool checkAlreadyInUse(const std::string& nickname) {
	for (size_t i = 0; i < Server::getInstance().getClients().size(); i++) {
		if (Server::getInstance().getClients()[i]->getNickname() == nickname) {
			return false;
		}
	}
	return true;
}

std::vector<Client*> NickCommand::getAffectedUsers(Client* client) const {
	std::vector<Channel*> channels = client->getJoinedChannels();
	std::vector<Client*> channelUsers;
	std::vector<Client*> affectedUsers;

	affectedUsers.push_back(client);

	for (size_t i = 0; i < channels.size(); ++i) {
		channelUsers = channels[i]->getUsers();
		for (size_t j = 0; j < channelUsers.size(); ++j) {
			if (std::find(affectedUsers.begin(), affectedUsers.end(),
						  channelUsers[j]) == affectedUsers.end())
				affectedUsers.push_back(channelUsers[j]);
		}
	}

	return affectedUsers;
}

void NickCommand::execute(Client* client, const std::string& args) {
	if (args.empty())
		return client->sendError(ERR_NONICKNAMEGIVEN,
								 client->getClientnickName(), _431);
	if (checkNicknameValid(args) == false)
		return client->sendError(ERR_ERRONEUSNICKNAME,
								 client->getClientnickName() + " " + args,
								 _432);
	if (checkAlreadyInUse(args) == false)
		return client->sendError(
			ERR_NICKNAMEINUSE, client->getClientnickName() + " " + args, _433);

	if (client->getStatus() == REGISTRED) {

		std::vector<Client*> affectedUsers = getAffectedUsers(client);

		for (size_t i = 0; i < affectedUsers.size(); ++i) {
			affectedUsers[i]->sendMessage(client->getPrefix(), "NICK", "",
										  args);
		}
	}

	client->setNickname(args);

	if (client->getStatus() != REGISTRED && client->getStatus() == PASSWD_OK)
		client->setStatus(NICK_OK);
}
