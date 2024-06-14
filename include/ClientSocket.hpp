#pragma once

#include "Socket.hpp"

class ClientSocket : public Socket {
  private:
	void onPoll();

  public:
	ClientSocket(int fd);
	~ClientSocket();
};
