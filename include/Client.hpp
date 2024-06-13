#pragma once

#include <string>

#include "SocketListener.hpp"

class Client : public SocketListener {

  private:
	int fd;
	std::string name;

	void onPoll();

  public:
	Client(int fd);
};
