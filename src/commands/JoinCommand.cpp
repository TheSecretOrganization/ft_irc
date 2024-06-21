#include "commands/JoinCommand.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"
#include <cstddef>
#include <cstdlib>

JoinCommand::JoinCommand() : Command("JOIN", 0, 1) {}

JoinCommand::~JoinCommand() {}


static std::map<std::string, std::string> channelMap(std::vector<std::string> vecChannels, std::string pass) {
	std::map<std::string, std::string> map;
	std::vector<std::string> vecPass = Command::split(pass, ',');

	if (vecChannels.size() < vecPass.size())
		return map;
	for (size_t i = 0; i < vecChannels.size(); i++)
	{
		map[vecChannels[i]] = (i < vecPass.size()) ? vecPass[i] : "";
	}
	return map;
}

std::vector<Channel*> JoinCommand::getTrueChannels(Client* client, std::map<std::string, std::string>& map) {
	std::vector<Channel*> channels;
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
	{
		channels.push_back(Server::getInstance().getChannel((*it).first));
		if (noSuchChannel(client, channels.back(), (*it).first)) {
			try
			{
				Channel::checkChannelSyntax((*it).first);
			}
			catch(const std::exception& e)
			{
				sendError(client, ERR_NOSUCHCHANNEL, _403, (*it).first);
				map.erase(it);
				channels.pop_back();
			}
		}
	}
	return channels;
}

bool	JoinCommand::badChannelKey(Client* client, Channel* channel, const std::string& password) {
	if (channel && channel->getChannelPassword() != password) {
		sendError(client, ERR_BADCHANNELKEY, _475, channel->getChannelName());
		return true;
	}
	return false;
}

bool	JoinCommand::isChannelFull(Client* client, Channel* channel) {
	if (channel->getUsers().size() >= channel->getChannelSize())
	{
		sendError(client, ERR_CHANNELISFULL, _471, channel->getChannelName());
		return true;
	}
	return false;
}

bool	JoinCommand::inviteOnlyChan(Client* client, Channel* channel) {
	if (channel->isInviteMode() && !channel->isUserInvited(client))
	{
		sendError(client, ERR_INVITEONLYCHAN, _473, channel->getChannelName());
		return true;
	}
	return false;
}

void	JoinCommand::execute(Client* client, std::string args) {
	if (args == "0") {
		// TODO: QUIT ALL CHANNELS;
		return;
	}
	std::vector<std::string> vecArgs = Command::split(args, ' ');
	if (needMoreParams(client, vecArgs)) {
		return ;
	}


	std::map<std::string, std::string> map = vecArgs.size() > 1
	? channelMap(split(vecArgs[0], ','), vecArgs[1])
	: channelMap(split(vecArgs[0], ','), "");

	if (map.size() > (size_t)std::atoi(Server::getInstance().getConfiguration().getValue("chanlimit").c_str())) {
		sendError(client, ERR_TOOMANYCHANNELS, _405);
	}

	std::vector<Channel*> channels = getTrueChannels(client, map);
	if (channels.empty())
		return ;

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i] == NULL)
			continue;
		if (badChannelKey(client, channels[i], map.at(channels[i]->getChannelName())))
		{
			map.erase(channels[i]->getChannelName());
			channels.erase(channels.begin() + i);
			i--;
			continue;
		}
	}

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i] == NULL)
			continue;
		if (isChannelFull(client, channels[i]))
		{
			map.erase(channels[i]->getChannelName());
			channels.erase(channels.begin() + i);
			i--;
			continue;
		}
	}

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i] == NULL)
			continue;
		if (inviteOnlyChan(client, channels[i]))
		{
			map.erase(channels[i]->getChannelName());
			channels.erase(channels.begin() + i);
			i--;
			continue;
		}
	}

	std::map<std::string, std::string>::iterator jt = map.begin();
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i] == NULL)
			Channel::createChannel(client, jt->first, jt->second);
		else
		{
			channels[i]->addUser(client);
		}
		jt++;
	}

	//TODO: replies
}
