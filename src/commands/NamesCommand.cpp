#include "commands/NamesCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <string>
#include <vector>

NamesCommand::NamesCommand() : Command("NAMES", 1, 0){};

NamesCommand::~NamesCommand() {}

std::string NamesCommand::getNames(Client* client, Channel* channel) const {
	std::string stringUsers = "";
	std::vector<Client*> users = channel->getUsers();

	for (std::vector<Client*>::iterator it = users.begin(); it != users.end();
		 ++it) {
		if (channel->isUserOnChannel(client) || !(*it)->isInvisible())
			stringUsers = stringUsers + " " +
						  (channel->isUserOperator(*it) ? "@" : "") +
						  (*it)->getNickname();
	}

	return stringUsers;
}

void NamesCommand::execute(Client* client, const std::string& args) {
	if (needMoreParams(client, split(args, ' ')))
		return;

	std::vector<std::string> splitArgs = split(args, ',');
	Channel* channel = NULL;
	for (size_t i = 0; i < splitArgs.size(); ++i) {
		channel = Server::getInstance().getChannel(splitArgs[i]);
		if (channel)
			client->sendMessage(Server::getInstance().getPrefix(), RPL_NAMREPLY,
								client->getClientnickName() + " = " +
									channel->getName(),
								getNames(client, channel));
		client->sendMessage(
			Server::getInstance().getPrefix(), RPL_ENDOFNAMES,
			client->getClientnickName() + " " + channel->getName(), _366);
	}
}
