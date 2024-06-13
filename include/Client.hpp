#pragma once

#include <string>

#include "PacketListener.hpp"

class Client : public PacketListener {

  private:
	int fd;
	std::string name;

	void onPacket(const std::string& content);

  public:
	Client(int fd);
};
