#pragma once

#include <string>

#include "SocketListener.hpp"

class PacketListener : public SocketListener {

  private:
	void onPoll();

  public:
	virtual void onPacket(const std::string& content) = 0;
};
