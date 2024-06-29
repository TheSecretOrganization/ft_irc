#include "commands/TopicCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <string>
#include <vector>

TopicCommand::TopicCommand() : Command("TOPIC", 0, 1) {}

TopicCommand::~TopicCommand() {}

void TopicCommand::execute(Client* client, const std::string& args) {
	std::vector<std::string> splitArgs = split(args, ':');

	if (needMoreParams(client, splitArgs))
		return;

	for (std::vector<std::string>::iterator it = splitArgs.begin();
		 it != splitArgs.end(); ++it) {
		*it = trim(*it);
	}

	Channel* channel = Server::getInstance().getChannel(splitArgs[0]);
	if (noSuchChannel(client, channel, splitArgs[0]))
		return;

	if (notOnChannel(client, channel))
		return;

	if (splitArgs.size() == 1) {
		if (channel->getTopic().empty())
			return channel->rplNoTopic(client);
		channel->rplTopic(client);
		return channel->rplTopicWhoTime(client);
	}

	if (channel->isTopicLocked() && !channel->isUserOperator(client))
		return client->sendError(
			ERR_CHANOPRIVSNEEDED,
			client->getClientnickName() + " " + channel->getName(), _482);

	channel->setTopic(client, splitArgs[1]);
}
