#include "commands/KickCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
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

	if (channel->isUserOnChannel(client) == false)
		return client->sendError(
			ERR_NOTONCHANNEL,
			client->getClientnickName() + " " + channel->getName(), _442);

	if (channel->isUserOperator(client) == false)
		return client->sendError(
			ERR_CHANOPRIVSNEEDED,
			client->getClientnickName() + " " + channel->getName(), _482);

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

		if (user == client) {
			client->sendError(ERR_CHANOPRIVSNEEDED,
							  client->getClientnickName() + " " +
								  channel->getName(),
							  "You cannot kick yourself");
			continue;
		}

		std::string message =
			(splitArgs.size() >= 3 && splitArgs[2].empty() == false &&
			 splitArgs[2] != ":")
				? splitArgs[2]
				: "By 42's will";

		channel->broadcast(client->getPrefix(), "KICK",
						   user->getClientnickName(), message);
		channel->removeUser(user);
	}
}
