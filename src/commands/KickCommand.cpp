#include "commands/KickCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

KickCommand::KickCommand() : Command("KICK", 0, 2) {}

KickCommand::~KickCommand() {}

void KickCommand::execute(Client* client, const std::string& args) {
	std::vector<std::string> splitArgs = split(args, ' ');

	if (needMoreParams(client, splitArgs))
		return;

	Channel* channel = Server::getInstance().getChannel(splitArgs[0]);
	if (noSuchChannel(client, channel, splitArgs[0]))
		return;

	if (notOnChannel(client, channel))
		return;

	if (chanOPrivsNeeded(client, channel))
		return;

	std::vector<std::string> usersNick = split(splitArgs[1], ',');
	Client* user = NULL;

	for (size_t i = 0; i < usersNick.size(); ++i) {
		user = Server::getInstance().getClient(usersNick[i]);

		if (user == NULL || channel->isUserOnChannel(user) == false) {
			client->sendError(ERR_USERNOTINCHANNEL,
							  client->getClientnickName() + " " + usersNick[i] +
								  " " + channel->getName(),
							  _441);
			continue;
		}

		if (user->isBot()) {
			client->sendError(ERR_CHANOPRIVSNEEDED,
							  client->getClientnickName() + " " +
								  channel->getName(),
							  "You cannot kick the Guardian");
			continue;
		}

		if (user == client) {
			client->sendError(ERR_CHANOPRIVSNEEDED,
							  client->getClientnickName() + " " +
								  channel->getName(),
							  "You cannot kick yourself");
			continue;
		}

		std::string message = "";

		for (size_t i = 2; i < splitArgs.size(); ++i) {
			message += splitArgs[i];
			if (i + 1 != splitArgs.size())
				message += " ";
		}

		if (message[0] == ':')
			message.erase(0, 1);

		if (message.empty())
			message = "By 42's will";

		channel->broadcast(client->getPrefix(), "KICK",
						   user->getClientnickName(), message);
		channel->removeUser(user);
	}

	if (channel->getUsers().size() - 1 == 0) {
		try {
			Server::getInstance().deleteChannel(channel);
		} catch (const Server::ChannelNotFoundException& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
