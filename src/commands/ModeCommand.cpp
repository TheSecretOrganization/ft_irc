#include "commands/ModeCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

ModeCommand::ModeCommand() : Command("MODE", 0, 1){};

ModeCommand::~ModeCommand() {}

bool ModeCommand::checkModes(Client* client) const {
	if (splitArgs[1][0] != '+' && splitArgs[1][0] != '-') {
		client->sendError(ERR_UMODEUNKNOWNFLAG, client->getClientnickName(),
						  _501);
		return false;
	}

	size_t n = 0;
	for (size_t i = 1; i < splitArgs.size(); ++i) {
		if (Server::getInstance().getConfiguration().getValue("cpmodes").find(
				splitArgs[1][i]) != std::string::npos) {
			++n;
			if (i + n > splitArgs.size() - 1) {
				client->sendError(ERR_NEEDMOREPARAMS,
								  client->getClientnickName() + " MODE", _461);
				return false;
			}
		}
	}

	return true;
}

void ModeCommand::parseModes(Client* client, Channel* channel) const {
	if (splitArgs.size() == 1)
		return client->sendMessage(
			Server::getInstance().getPrefix(), RPL_CHANNELMODEIS,
			client->getClientnickName() + " " + channel->getName() + " " +
				channel->getModes());

	if (!checkModes(client))
		return;

	if (!channel->isUserOperator(client))
		return client->sendError(
			ERR_CHANOPRIVSNEEDED,
			client->getClientnickName() + " " + channel->getName(), _482);

	size_t n = 2;
	for (size_t i = 1; splitArgs[1][i]; ++i) {
		if (Server::getInstance().getConfiguration().getValue("cpmodes").find(
				splitArgs[1][i]) != std::string::npos) {

			setMode(client, channel, splitArgs[1][0] == '+', splitArgs[1][i],
					splitArgs[n]);
			++n;
		} else {
			setMode(client, channel, splitArgs[1][0] == '+', splitArgs[1][i]);
		}
	}
}

void ModeCommand::parseModes(Client* client) const {
	if (splitArgs.size() == 1)
		return client->sendMessage(
			Server::getInstance().getPrefix(), RPL_UMODEIS,
			client->getClientnickName() + " " + client->getModes());

	if (!checkModes(client))
		return;

	for (size_t i = 1; splitArgs[1][i]; ++i) {
		setMode(client, splitArgs[1][0] == '+', splitArgs[1][i]);
	}
}

void ModeCommand::setMode(Client* client, bool action, char mode,
						  const std::string& param) const {
	(void)param;
	if (mode == 'a')
		client->setAway(action);
	else {
		client->sendError(ERR_UMODEUNKNOWNFLAG,
						  client->getClientnickName() + " " + mode, _501);
		return;
	}
	client->sendMessage(Server::getInstance().getPrefix(), name,
						client->getClientnickName() + " " +
							(action ? "+" : "-") + std::string(1, mode));
}

void ModeCommand::setMode(Client* client, Channel* channel, bool action,
						  char mode, const std::string& param) const {
	if (mode == 'i')
		channel->setInviteMode(action);
	else if (mode == 't')
		channel->setTopicLocked(action);
	else if (mode == 'k')
		channel->setPassword(action ? param : "");
	else if (mode == 'o') {
		Client* target = Server::getInstance().getClient(param);
		if (!target)
			return client->sendError(ERR_NOSUCHNICK,
									 client->getClientnickName() + " " + param,
									 _401);
		if (!channel->isUserOnChannel(target))
			return client->sendError(ERR_USERNOTINCHANNEL,
									 client->getClientnickName() + " " + param +
										 " " + channel->getName(),
									 _441);
		channel->addOperator(target);
	} else if (mode == 'l')
		channel->setUserLimit(action ? std::atoi(param.c_str()) : 0);
	else {
		client->sendError(ERR_UMODEUNKNOWNFLAG,
						  client->getClientnickName() + " " + mode, _501);
		return;
	}

	channel->broadcast(client->getPrefix(), name,
					   (action ? "+" : "-") + std::string(1, mode));
}

void ModeCommand::execute(Client* client, const std::string& args) {
	splitArgs = split(args, ' ');
	if (needMoreParams(client, splitArgs)) {
		return;
	}

	if (splitArgs[0][0] == '#') {
		Channel* channel = Server::getInstance().getChannel(splitArgs[0]);
		if (!channel)
			return client->sendError(
				ERR_NOSUCHCHANNEL,
				client->getClientnickName() + " " + splitArgs[0], _403);

		parseModes(client, channel);
	} else {
		try {
			Server::getInstance().getClient(splitArgs[0]);
		} catch (const Server::ClientNotFoundException& e) {
			(void)e;
			return client->sendError(
				ERR_NOSUCHNICK,
				client->getClientnickName() + " " + splitArgs[0], _401);
		}

		if (client->getNickname() != splitArgs[0])
			return client->sendError(ERR_USERSDONTMATCH,
									 client->getClientnickName(), _502);

		parseModes(client);
	}
}
