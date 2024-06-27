#include "commands/JoinCommand.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

JoinCommand::JoinCommand() : Command("JOIN", 0, 1) {}

JoinCommand::~JoinCommand() {}

static std::map<std::string, std::string>
channelMap(std::vector<std::string> vecChannels, std::string pass) {
	std::map<std::string, std::string> map;
	std::vector<std::string> vecPass = Command::split(pass, ',');

	if (vecChannels.size() < vecPass.size())
		return map;
	for (size_t i = 0; i < vecChannels.size(); i++) {
		map[vecChannels[i]] = (i < vecPass.size()) ? vecPass[i] : "";
	}
	return map;
}

std::vector<Channel*>
JoinCommand::getTrueChannels(Client* client,
							 std::map<std::string, std::string>& map) const {
	std::vector<Channel*> channels;
	for (std::map<std::string, std::string>::iterator it = map.begin();
		 it != map.end(); it++) {
		channels.push_back(Server::getInstance().getChannel((*it).first));
		if (noSuchChannel(client, channels.back(), (*it).first)) {
			try {
				Channel::checkChannelSyntax((*it).first);
			} catch (const std::exception& e) {
				client->sendError(
					ERR_NOSUCHCHANNEL,
					client->getClientnickName() + " " + (*it).first, _403);
				map.erase(it);
				channels.pop_back();
			}
		}
	}
	return channels;
}

bool JoinCommand::badChannelKey(Client* client, Channel* channel,
								const std::string& password) const {
	if (channel && channel->getChannelPassword() != password) {
		client->sendError(ERR_BADCHANNELKEY,
						  client->getClientnickName() + " " +
							  channel->getChannelName(),
						  _475);
		return true;
	}
	return false;
}

bool JoinCommand::isChannelFull(Client* client, Channel* channel) const {
	if (channel->getUsers().size() >= channel->getChannelSize()) {
		client->sendError(ERR_CHANNELISFULL,
						  client->getClientnickName() + " " +
							  channel->getChannelName(),
						  _471);
		return true;
	}
	return false;
}

bool JoinCommand::inviteOnlyChan(Client* client, Channel* channel) const {
	if (channel->isInviteMode() && !channel->isUserInvited(client)) {
		client->sendError(ERR_INVITEONLYCHAN,
						  client->getClientnickName() + " " +
							  channel->getChannelName(),
						  _473);
		return true;
	}
	return false;
}

std::string JoinCommand::getUsersString(std::vector<Client*>& users) const {
	std::string stringUsers;

	for (std::vector<Client*>::iterator it = users.begin(); it != users.end();
		 ++it) {
		stringUsers = stringUsers + " " + (*it)->getNickname();
	}

	return stringUsers;
}

void JoinCommand::sendReplies(Client* client, Channel* channel) const {
	client->sendMessage("JOIN", "", channel->getChannelName());
	if (!channel->getTopic().empty())
		client->sendMessage(RPL_TOPIC, channel->getTopic(),
							client->getNickname() + " " +
								channel->getChannelName());
	client->sendMessage(RPL_NAMREPLY, getUsersString(channel->getUsers()),
						client->getNickname() + " = " +
							channel->getChannelName());
	client->sendMessage(RPL_ENDOFNAMES, _366,
						client->getNickname() + " " +
							channel->getChannelName());
	for (std::vector<Client*>::iterator it = channel->getUsers().begin();
		 it != channel->getUsers().end(); ++it) {
		if ((*it) == client)
			continue;
		(*it)->sendMessage(client->getFormatUser(), "",
						   "JOIN " + channel->getChannelName());
	}
}

void JoinCommand::execute(Client* client, std::string args) {
	if (args == "0") {
		// TODO: QUIT ALL CHANNELS;
		return;
	}
	std::vector<std::string> vecArgs = Command::split(args, ' ');
	if (needMoreParams(client, vecArgs)) {
		return;
	}

	std::map<std::string, std::string> map =
		vecArgs.size() > 1 ? channelMap(split(vecArgs[0], ','), vecArgs[1])
						   : channelMap(split(vecArgs[0], ','), "");

	if (map.size() > (size_t)std::atoi(Server::getInstance()
										   .getConfiguration()
										   .getValue("chanlimit")
										   .c_str())) {
		client->sendError(ERR_TOOMANYCHANNELS, client->getClientnickName(),
						  _405);
	}

	std::vector<Channel*> channels = getTrueChannels(client, map);
	if (channels.empty())
		return;

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i] == NULL)
			continue;
		if (badChannelKey(client, channels[i],
						  map.at(channels[i]->getChannelName()))) {
			map.erase(channels[i]->getChannelName());
			channels.erase(channels.begin() + i);
			i--;
			continue;
		}
	}

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i] == NULL)
			continue;
		if (isChannelFull(client, channels[i])) {
			map.erase(channels[i]->getChannelName());
			channels.erase(channels.begin() + i);
			i--;
			continue;
		}
	}

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i] == NULL)
			continue;
		if (inviteOnlyChan(client, channels[i])) {
			map.erase(channels[i]->getChannelName());
			channels.erase(channels.begin() + i);
			i--;
			continue;
		}
	}

	std::map<std::string, std::string>::iterator jt = map.begin();
	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i] == NULL) {
			Channel::createChannel(client, jt->first, jt->second);
			channels[i] = Server::getInstance().getChannel(jt->first);
		} else {
			channels[i]->addUser(client);
		}

		try {
			sendReplies(client, channels[i]);
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}

		jt++;
	}

	// TODO: replies
}
