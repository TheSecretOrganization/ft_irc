#include "commands/JoinCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"
#include "commands/NamesCommand.hpp"
#include "commands/PartCommand.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

JoinCommand::JoinCommand() : Command("JOIN", 0, 1) {}

JoinCommand::~JoinCommand() {}

bool JoinCommand::badChannelKey(Client* client, Channel const* channel,
								const std::string& password) const {
	if (channel && channel->getPassword() != password) {
		client->sendError(
			ERR_BADCHANNELKEY,
			client->getClientnickName() + " " + channel->getName(), _475);
		return true;
	}
	return false;
}

bool JoinCommand::isChannelFull(Client* client, Channel* channel) const {
	if (channel->getUserLimit() != 0 &&
		channel->getUsers().size() >= channel->getUserLimit()) {
		client->sendError(
			ERR_CHANNELISFULL,
			client->getClientnickName() + " " + channel->getName(), _471);
		return true;
	}
	return false;
}

bool JoinCommand::inviteOnlyChan(Client* client, Channel* channel) const {
	if (channel->isInviteMode() && !channel->isUserInvited(client)) {
		client->sendError(
			ERR_INVITEONLYCHAN,
			client->getClientnickName() + " " + channel->getName(), _473);
		return true;
	}
	return false;
}

void JoinCommand::sendReplies(Client* client, size_t i) const {
	client->sendMessage(client->getPrefix(), "JOIN",
						channels[i].ptr->getName());

	if (!channels[i].ptr->getTopic().empty())
		client->sendMessage(Server::getInstance().getPrefix(), RPL_TOPIC,
							client->getNickname() + " " +
								channels[i].ptr->getName(),
							channels[i].ptr->getTopic());

	NamesCommand().execute(client, channels[i].ptr->getName());

	for (std::vector<Client*>::iterator it =
			 channels[i].ptr->getUsers().begin();
		 it != channels[i].ptr->getUsers().end(); ++it) {
		if ((*it) == client)
			continue;
		(*it)->sendMessage(client->getPrefix(), "JOIN",
						   channels[i].ptr->getName());
	}
}

void JoinCommand::joinZero(Client* client) const {
	std::vector<Channel*> chans = client->getJoinedChannels();
	for (size_t i = 0; i < chans.size(); ++i) {
		if (chans[i]->isUserOnChannel(client)) {
			PartCommand().execute(client, chans[i]->getName());
		}
	}
}

bool JoinCommand::splitArgs(Client* client, const std::string& args) {
	std::vector<std::string> names = Command::split(args, ' ');

	if (needMoreParams(client, names))
		return false;

	std::vector<std::string> keys;
	if (names.size() > 1)
		keys = split(names[1], ',');

	names = split(names[0], ',');

	for (size_t i = 0; i < names.size(); ++i) {
		std::cout << names[i] << std::endl;
	}

	for (size_t i = 0; i < names.size(); ++i) {
		channels.push_back(
			s_channel(Server::getInstance().getChannel(channels[i].name),
					  names[i], keys.size() ? keys[i] : ""));

		if (channels[i].ptr == NULL) {
			try {
				Channel::checkChannelSyntax(channels[i].name);
			} catch (const std::exception& e) {
				client->sendError(
					ERR_NOSUCHCHANNEL,
					client->getClientnickName() + " " + channels[i].name, _403);
				channels[i].name = "";
			}
		} else {
			if (badChannelKey(client, channels[i].ptr, channels[i].key) ||
				isChannelFull(client, channels[i].ptr) ||
				inviteOnlyChan(client, channels[i].ptr))
				channels[i].name = "";
		}
	}

	return true;
}

void JoinCommand::execute(Client* client, const std::string& args) {
	if (args == "0")
		return joinZero(client);

	if (!splitArgs(client, args) || channels.empty())
		return;

	for (size_t i = 0; i < channels.size(); ++i) {
		if (channels[i].name.empty())
			continue;

		if (client->getJoinedChannels().size() >=
			(size_t)std::atoi(Server::getInstance()
								  .getConfiguration()
								  .getValue("chanlimit")
								  .c_str())) {
			client->sendError(
				ERR_TOOMANYCHANNELS,
				client->getClientnickName() + " " + channels[i].name, _405);
			continue;
		}

		if (channels[i].ptr == NULL) {
			Channel::createChannel(client, channels[i].name, channels[i].key);
			channels[i].ptr =
				Server::getInstance().getChannel(channels[i].name);
		} else {
			if (channels[i].ptr->isUserOnChannel(client)) {
				client->sendError(
					ERR_USERONCHANNEL,
					client->getClientnickName() + " " + channels[i].name, _443);
				continue;
			} else if (channels[i].ptr->isUserBanned(client)) {
				client->sendError(
					ERR_BANNEDFROMCHAN,
					client->getClientnickName() + " " + channels[i].name, _474);
				continue;
			}
			channels[i].ptr->addUser(client);
		}
		sendReplies(client, i);
	}
}
