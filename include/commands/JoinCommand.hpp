#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <cstddef>
#include <string>
#include <vector>

typedef struct s_channel {
	Channel* ptr;
	std::string name;
	std::string key;

	s_channel() : ptr(NULL), name(""), key("") {}
	s_channel(Channel* ptr, const std::string& name, const std::string& key)
		: ptr(ptr), name(name), key(key) {}
} channel_t;

class JoinCommand : public Command {
  private:
	std::vector<channel_t> channels;
	bool badChannelKey(Client* client, Channel const* channel,
					   const std::string& password) const;
	bool isChannelFull(Client* client, Channel* channel) const;
	bool inviteOnlyChan(Client* client, Channel* channel) const;
	void sendReplies(Client* client, size_t i) const;
	void joinZero(Client* client) const;
	bool splitArgs(Client* client, const std::string& args);

  public:
	JoinCommand();
	~JoinCommand();

	void execute(Client* client, const std::string& args);
};
