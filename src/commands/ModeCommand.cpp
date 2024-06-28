#include "commands/ModeCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

ModeCommand::ModeCommand() : Command("MODE", 0, 1){};

ModeCommand::~ModeCommand() {}

bool ModeCommand::checkModes(Client* client) const {
	if (args[1][0] != '+' && args[1][0] != '-') {
		client->sendError(ERR_UMODEUNKNOWNFLAG, client->getClientnickName(),
						  _501);
		return false;
	}

	size_t n = 0;
	for (size_t i = 1; i < args.size(); ++i) {
		if (Server::getInstance().getConfiguration().getValue("cpmodes").find(
				args[1][i]) != std::string::npos) {
			++n;
			std::cout << "size: " << args.size() << " " << i + n << std::endl;
			if (i + n > args.size() - 1) {
				client->sendError(ERR_NEEDMOREPARAMS,
								  client->getClientnickName() + " MODE", _461);
				return false;
			}
		}
	}

	return true;
}

void ModeCommand::parseModes(Client* client, Channel* channel) {
	if (args.size() == 1)
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
	for (size_t i = 1; args[1][i]; ++i) {
		if (Server::getInstance().getConfiguration().getValue("cpmodes").find(
				args[1][i]) != std::string::npos)
			setMode(client, channel, args[1][0] == '+', args[1][i], args[n++]);
		else
			setMode(client, channel, args[1][0] == '+', args[1][i]);
	}
}

void ModeCommand::parseModes(Client* client) {
	if (args.size() == 1)
		return client->sendMessage(
			Server::getInstance().getPrefix(), RPL_UMODEIS,
			client->getClientnickName() + " " + client->getModes());

	if (!checkModes(client))
		return;

	for (size_t i = 1; args[1][i]; ++i) {
		setMode(client, args[1][0] == '+', args[1][i]);
	}
}

void ModeCommand::setMode(Client* client, bool action, char mode,
						  const std::string& param) {
	(void)param;
	switch (mode) {
	case 'a':
		client->setAway(action);
		break;
	default:
		client->sendError(ERR_UMODEUNKNOWNFLAG,
						  client->getClientnickName() + " " + mode, _501);
		break;
	}
}

void ModeCommand::setMode(Client* client, Channel* channel, bool action,
						  char mode, const std::string& param) {
	Client* target = NULL;
	switch (mode) {
	case 'i':
		channel->setInviteMode(action);
		break;
	case 't':
		channel->setTopicLocked(action);
		break;
	case 'k':
		channel->setPassword(action ? param : "");
		break;
	case 'o':
		target = Server::getInstance().getClient(param);
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
		break;
	case 'l':
		channel->setUserLimit(action ? std::atoi(param.c_str()) : 0);
		break;
	default:
		client->sendError(ERR_UMODEUNKNOWNFLAG,
						  client->getClientnickName() + " " + mode, _501);
		break;
	}
}

void ModeCommand::execute(Client* client, std::string args) {
	this->args = split(args, ' ');
	if (needMoreParams(client, this->args)) {
		return;
	}

	if (this->args[0][0] == '#') {
		Channel* channel = Server::getInstance().getChannel(this->args[0]);
		if (!channel)
			return client->sendError(
				ERR_NOSUCHCHANNEL,
				client->getClientnickName() + " " + this->args[0], _403);

		parseModes(client, channel);
	} else {
		try {
			Server::getInstance().getClient(this->args[0]);
		} catch (const Server::ClientNotFoundException& e) {
			(void)e;
			return client->sendError(
				ERR_NOSUCHNICK,
				client->getClientnickName() + " " + this->args[0], _401);
		}

		if (client->getNickname() != this->args[0])
			return client->sendError(ERR_USERSDONTMATCH,
									 client->getClientnickName(), _502);

		parseModes(client);
	}
}
