#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>
#include <vector>

typedef struct s_mode {
	bool action;
	char name;
	std::string param;

	s_mode() : action(false), name('\0'), param("") {}
	s_mode(bool action, char name, std::string param)
		: action(action), name(name), param(param) {}
} t_mode;

class ModeCommand : public Command {
  private:
	bool createModes(Client* client, std::vector<t_mode>& modes,
					 const std::vector<std::string>& splitArgs) const;
	void sendModes(Client* client, Channel* channel) const;
	void sendBanList(Client* client, Channel* channel) const;
	void setMode(Client* client, const t_mode& mode) const;
	void setMode(Client* client, Channel* channel, const t_mode& mode) const;
	bool setBan(Client* client, Channel* channel, const t_mode& mode) const;
	bool setOperator(Client* client, Channel* channel,
					 const t_mode& mode) const;

  public:
	ModeCommand();
	~ModeCommand();

	void execute(Client* client, const std::string& args);
};
