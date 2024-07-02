#include "commands/PartCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
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
		if (noSuchChannel(client, channel, channels[i]))
			return;

		if (notOnChannel(client, channel))
			return;

		channel->broadcast(client->getPrefix(), "PART", "",
						   splitArgs.size() >= 2 ? splitArgs[1] : "Bye!");
		try {
			channel->removeUser(client);
		} catch (const Server::ChannelNotFoundException& e) {
			std::cerr << e.what() << std::endl;
		}

		if (channel->getUsers().size() - 1 == 0) {
			try {
				Server::getInstance().deleteChannel(channel);
			} catch (const Server::ChannelNotFoundException& e) {
				std::cerr << e.what() << std::endl;
			}
		}
	}
}
