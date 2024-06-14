#pragma once

#include "Socket.hpp"

class ServerSocket : public Socket {
  private:
	int port;

	void onPoll();

  public:
	ServerSocket();
	~ServerSocket();

	void init(int port);
};
