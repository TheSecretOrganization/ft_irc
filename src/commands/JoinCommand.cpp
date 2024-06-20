#include "JoinCommand.hpp"
#include "IrcReplies.hpp"

JoinCommand::JoinCommand() {
	command = "JOIN";
	expectedSize = 0;
	minSize = 1;
}

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

bool	JoinCommand::checkChannelKey(Client* client, Channel* channel, const std::string& password) {
	if (channel && channel->getChannelPassword() != password) {
		sendError(client, ERR_BADCHANNELKEY, _475, channel->getChannelName());
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

	if (map.size() > stringToSizeT(Server::getInstance().getConfiguration().getValue("chanlimit"))) {
		sendError(client, ERR_TOOMANYCHANNELS, _405);
	}

	std::vector<Channel*> channels = getTrueChannels(client, map);
	if (channels.empty())
		return ;

	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i] == NULL)
			continue;
		checkChannelKey(client, channels[i], map.at(channels[i]->getChannelName()));
	}
}
