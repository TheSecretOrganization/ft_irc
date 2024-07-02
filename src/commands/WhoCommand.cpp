#include "commands/WhoCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"
#include <cstddef>
#include <vector>

WhoCommand::WhoCommand() : Command("WHO", 1, 0) {}

WhoCommand::~WhoCommand() {}

std::string WhoCommand::getUserInfo(Client* user, bool isUserOperator) const {
	return "~" + user->getUsername() + " " + user->getHostname() + " " +
		   user->getServername() + " " + user->getClientnickName() +
		   (user->isAway() ? " G" : " H") + (isUserOperator ? "@" : "");
}

void WhoCommand::channel(Client* client, const std::string& args) const {
	Channel* chan = Server::getInstance().getChannel(args);
	if (noSuchChannel(client, chan, args))
		return;

	Client* user = NULL;
	for (size_t i = 0; i < chan->getUsers().size(); ++i) {
		user = chan->getUsers()[i];
		if (user->isInvisible() && !chan->isUserOnChannel(client))
			continue;
		client->sendMessage(Server::getInstance().getPrefix(), RPL_WHOREPLY,
							client->getClientnickName() + " " +
								chan->getName() + " " +
								getUserInfo(user, chan->isUserOperator(user)),
							"0 " + user->getRealname());
	}
	client->sendMessage(Server::getInstance().getPrefix(), RPL_ENDOFWHO,
						client->getClientnickName() + " " + chan->getName(),
						_315);
}

void WhoCommand::user(Client* client, const std::string& args) const {
	Client* target = Server::getInstance().getClient(args);
	if (target) {
		bool isUserOperator = false;
		bool sharedChannel = false;
		std::vector<Channel*> channels = target->getJoinedChannels();

		for (size_t i = 0; i < channels.size(); ++i) {
			if (!isUserOperator && channels[i]->isUserOperator(target))
				isUserOperator = true;
			if (!sharedChannel && channels[i]->isUserOnChannel(client))
				sharedChannel = true;
		}

		if (!target->isInvisible() || sharedChannel || target == client)
			client->sendMessage(Server::getInstance().getPrefix(), RPL_WHOREPLY,
								client->getClientnickName() + " * " +
									getUserInfo(target, isUserOperator),
								"0 " + target->getRealname());
	}
	client->sendMessage(Server::getInstance().getPrefix(), RPL_ENDOFWHO,
						client->getClientnickName(), _315);
}

void WhoCommand::execute(Client* client, const std::string& args) {
	if (needMoreParams(client, split(args, ' ')))
		return;

	if (args[0] == '#') {
		channel(client, args);
	} else {
		user(client, args);
	}
}
