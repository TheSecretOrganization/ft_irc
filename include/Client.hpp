#pragma once

#include <string>

#include "PacketSubscriber.hpp"

class Client: public PacketSubscriber {

private:
	int fd;
	std::string name;

	void onPacket(const std::string &content);

public:
	Client(const std::string &name, int fd);

};
