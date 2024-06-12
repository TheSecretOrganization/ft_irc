#pragma once

#include <string>

#include "SocketSubscriber.hpp"

class PacketSubscriber : public SocketSubscriber {

  private:
	void onPoll();

  public:
	virtual void onPacket(const std::string& content) = 0;
};
