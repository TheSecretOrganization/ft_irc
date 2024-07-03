#include "commands/ModeCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

ModeCommand::ModeCommand() : Command("MODE", 0, 1){};

ModeCommand::~ModeCommand() {}

bool ModeCommand::createModes(Client* client, std::vector<t_mode>& modes,
							  const std::vector<std::string>& splitArgs) const {
	if (splitArgs[1][0] != '+' && splitArgs[1][0] != '-') {
		client->sendError(ERR_UMODEUNKNOWNFLAG, client->getClientnickName(),
						  _501);
		return false;
	}

	for (size_t i = 1; i < splitArgs[1].size(); ++i) {
		modes.push_back(
			t_mode(splitArgs[1][0] == '+', splitArgs[1][i],
				   i + 1 < splitArgs.size() ? splitArgs[i + 1] : ""));
	}

	return true;
}

void ModeCommand::sendModes(Client* client, Channel* channel) const {
	return channel
			   ? client->sendMessage(
					 Server::getInstance().getPrefix(), RPL_CHANNELMODEIS,
					 client->getClientnickName() + " " + channel->getName() +
						 " " + channel->getModes())
			   : client->sendMessage(
					 Server::getInstance().getPrefix(), RPL_UMODEIS,
					 client->getClientnickName() + " " + client->getModes());
}

void ModeCommand::sendBanList(Client* client, Channel* channel) const {
	std::string banList = "";

	for (size_t i = 0; i < channel->getBans().size(); ++i) {
		banList += channel->getBans()[i] + " ";
	}

	client->sendMessage(Server::getInstance().getPrefix(), RPL_BANLIST,
						client->getClientnickName() + " " + channel->getName(),
						banList);
	client->sendMessage(Server::getInstance().getPrefix(), RPL_ENDOFBANLIST,
						client->getClientnickName() + " " + channel->getName(),
						_368);
}

void ModeCommand::setMode(Client* client, const t_mode& mode) const {
	if (mode.name == 'a')
		client->setAway(mode.action);
	else if (mode.name == 'i')
		client->setInvisible(mode.action);
	else {
		client->sendError(ERR_UMODEUNKNOWNFLAG,
						  client->getClientnickName() + " " + mode.name, _501);
		return;
	}
	client->sendMessage(Server::getInstance().getPrefix(), name,
						client->getClientnickName() + " " +
							(mode.action ? "+" : "-") +
							std::string(1, mode.name));
}

void ModeCommand::setMode(Client* client, Channel* channel,
						  const t_mode& mode) const {
	if (mode.name == 'i')
		channel->setInviteMode(mode.action);
	else if (mode.name == 't')
		channel->setTopicLocked(mode.action);
	else if (mode.name == 'k') {
		if (mode.action && mode.param.empty())
			return client->sendError(ERR_NEEDMOREPARAMS,
									 client->getClientnickName() + " " + name,
									 _461);
		channel->setPassword(mode.action ? mode.param : "");
	} else if (mode.name == 'l') {
		if (mode.action && mode.param.empty())
			return client->sendError(ERR_NEEDMOREPARAMS,
									 client->getClientnickName() + " " + name,
									 _461);
		channel->setUserLimit(mode.action ? std::atoi(mode.param.c_str()) : 0);
	} else if (mode.name == 'o') {
		if (!setOperator(client, channel, mode))
			return;
	} else if (mode.name == 'b') {
		if (!setBan(client, channel, mode))
			return;
	} else
		return client->sendError(ERR_UMODEUNKNOWNFLAG,
								 client->getClientnickName() + " " + mode.name,
								 _501);

	channel->broadcast(client->getPrefix(), name,
					   (mode.action ? "+" : "-") + std::string(1, mode.name),
					   mode.param);
}

bool ModeCommand::setBan(Client* client, Channel* channel,
						 const t_mode& mode) const {
	if (mode.param.empty() || !channel->checkBanSyntax(mode.param)) {
		client->sendError(ERR_NEEDMOREPARAMS,
						  client->getClientnickName() + " " + name, _461);
		return false;
	}

	mode.action ? channel->addBan(mode.param) : channel->deleteBan(mode.param);
	sendBanList(client, channel);
	return true;
}

bool ModeCommand::setOperator(Client* client, Channel* channel,
							  const t_mode& mode) const {
	if (mode.param.empty()) {
		client->sendError(ERR_NEEDMOREPARAMS,
						  client->getClientnickName() + " " + name, _461);
		return false;
	}

	Client* target = Server::getInstance().getClient(mode.param);

	if (!target) {
		client->sendError(ERR_NOSUCHNICK,
						  client->getClientnickName() + " " +
							  channel->getName() + " " + mode.param,
						  _401);
		return false;
	}

	if (!channel->isUserOnChannel(target)) {
		client->sendError(ERR_USERNOTINCHANNEL,
						  client->getClientnickName() + " " +
							  channel->getName() + " " + mode.param,
						  _441);
		return false;
	}

	if (target->isBot()) {
		client->sendError(
			ERR_CHANOPRIVSNEEDED,
			client->getClientnickName() + " " + channel->getName(),
			"You are not permitted to remove the Guardian's powers");
		return false;
	}

	try {
		mode.action ? channel->addOperator(target)
					: channel->removeOperator(target);
	} catch (const std::logic_error& e) {
		(void)e;
		return false;
	}

	return true;
}

void ModeCommand::execute(Client* client, const std::string& args) {
	std::vector<std::string> splitArgs = split(args, ' ');
	if (needMoreParams(client, splitArgs))
		return;

	Channel* channel = NULL;

	if (splitArgs[0][0] == '#') {
		channel = Server::getInstance().getChannel(splitArgs[0]);
		if (!channel)
			return client->sendError(
				ERR_NOSUCHCHANNEL,
				client->getClientnickName() + " " + splitArgs[0], _403);
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
	}

	if (splitArgs.size() == 1)
		return sendModes(client, channel);

	if (channel && splitArgs[1] == "b")
		return sendBanList(client, channel);

	std::vector<t_mode> modes;
	if (!createModes(client, modes, splitArgs))
		return;

	if (channel && !channel->isUserOperator(client))
		return client->sendError(
			ERR_CHANOPRIVSNEEDED,
			client->getClientnickName() + " " + channel->getName(), _482);

	for (size_t i = 0; i < modes.size(); ++i) {
		channel ? setMode(client, channel, modes[i])
				: setMode(client, modes[i]);
	}
}
