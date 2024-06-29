#include "commands/WhoCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"
#include <cstddef>

WhoCommand::WhoCommand() : Command("WHO", 1, 0) {}

WhoCommand::~WhoCommand() {}

void WhoCommand::execute(Client* client, std::string args) {
	if (needMoreParams(client, split(args, ' ')))
		return;

	Channel* channel = Server::getInstance().getChannel(args);
	if (noSuchChannel(client, channel, args))
		return;

	Client* user = NULL;
	for (size_t i = 0; i < channel->getUsers().size(); ++i) {
		user = channel->getUsers()[i];
		client->sendMessage(Server::getInstance().getPrefix(), RPL_WHOREPLY,
							client->getClientnickName() + " " +
								channel->getName() + " " + user->getUsername() +
								" " + user->getHostname() + " " +
								user->getClientnickName(),
							user->getRealname());
	}

    client->sendMessage(Server::getInstance().getPrefix(), RPL_ENDOFWHO, client->getClientnickName() + " " + channel->getName(), _315);
}
