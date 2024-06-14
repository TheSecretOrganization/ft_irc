#pragma once

#include "Socket.hpp"

#include <string>

class ClientSocket : public Socket {
  private:
	void onPoll();

  public:
	ClientSocket(int fd);
	~ClientSocket();

	void sendPacket(std::string packet) const;
};
