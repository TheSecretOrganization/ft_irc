#include "commands/PartCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

PartCommand::PartCommand() : Command("PART", 0, 1) {}

PartCommand::~PartCommand() {}

void PartCommand::execute(Client* client, const std::string& args) {
	std::vector<std::string> splitArgs = split(args, ' ');

	if (needMoreParams(client, splitArgs))
		return;

	std::vector<std::string> channels = split(splitArgs[0], ',');
	Channel* channel = NULL;

	for (size_t i = 0; i < channels.size(); ++i) {
		channel = Server::getInstance().getChannel(channels[i]);
		if (!channel)
			return client->sendError(
				ERR_NOSUCHCHANNEL,
				client->getClientnickName() + " " + channels[i], _403);

		if (!channel->isUserOnChannel(client))
			return client->sendError(
				ERR_NOTONCHANNEL,
				client->getClientnickName() + " " + channel->getName(), _442);

		channel->broadcast(client->getPrefix(), "PART", "",
						   splitArgs.size() >= 2 ? splitArgs[1] : "Bye!");
        try {
            channel->removeUser(client);
        } catch (const Server::ChannelNotFoundException& e) {
            std::cerr << e.what() << std::endl;
        }
	}
}
