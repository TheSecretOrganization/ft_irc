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
		if (!channels.back()) {
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

void JoinCommand::sendReplies(Client* client, Channel* channel) const {
	client->sendMessage(client->getPrefix(), "JOIN", channel->getName());

	if (!channel->getTopic().empty())
		client->sendMessage(Server::getInstance().getPrefix(), RPL_TOPIC,
							client->getNickname() + " " + channel->getName(),
							channel->getTopic());

	NamesCommand().execute(client, channel->getName());

	for (std::vector<Client*>::iterator it = channel->getUsers().begin();
		 it != channel->getUsers().end(); ++it) {
		if ((*it) == client)
			continue;
		(*it)->sendMessage(client->getPrefix(), "JOIN", channel->getName());
	}
}

void JoinCommand::joinZero(Client* client) const {
	std::vector<Channel*> channels = client->getJoinedChannels();
	for (size_t i = 0; i < channels.size(); ++i) {
		if (channels[i]->isUserOnChannel(client)) {
			PartCommand().execute(client, channels[i]->getName());
		}
	}
}

void JoinCommand::execute(Client* client, const std::string& args) {
	if (args == "0")
		return joinZero(client);

	std::vector<std::string> vecArgs = Command::split(args, ' ');
	if (needMoreParams(client, vecArgs)) {
		return;
	}

	std::map<std::string, std::string> map =
		vecArgs.size() > 1 ? channelMap(split(vecArgs[0], ','), vecArgs[1])
						   : channelMap(split(vecArgs[0], ','), "");

	std::vector<Channel*> channels = getTrueChannels(client, map);
	if (channels.empty())
		return;

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i] == NULL)
			continue;
		if (badChannelKey(client, channels[i],
						  map.at(channels[i]->getName())) ||
			isChannelFull(client, channels[i]) ||
			inviteOnlyChan(client, channels[i])) {
			map.erase(channels[i]->getName());
			channels.erase(channels.begin() + i);
			i--;
			continue;
		}
	}

	std::map<std::string, std::string>::iterator jt = map.begin();
	for (size_t i = 0; i < channels.size(); i++) {
		if (client->getJoinedChannels().size() >=
			(size_t)std::atoi(Server::getInstance()
								  .getConfiguration()
								  .getValue("chanlimit")
								  .c_str())) {
			client->sendError(ERR_TOOMANYCHANNELS,
							  client->getClientnickName() + " " + jt->first,
							  _405);
			continue;
		}

		if (channels[i] == NULL) {
			Channel::createChannel(client, jt->first, jt->second);
			channels[i] = Server::getInstance().getChannel(jt->first);
		} else {
			if (channels[i]->isUserOnChannel(client)) {
				client->sendError(ERR_USERONCHANNEL,
								  client->getClientnickName() + " " +
									  channels[i]->getName(),
								  _443);
				continue;
			} else if (channels[i]->isUserBanned(client)) {
				client->sendError(ERR_BANNEDFROMCHAN,
								  client->getClientnickName() + " " +
									  channels[i]->getName(),
								  _474);
				continue;
			}
			channels[i]->addUser(client);
		}
		sendReplies(client, channels[i]);
		jt++;
	}
}
