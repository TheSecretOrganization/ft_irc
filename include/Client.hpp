#pragma once

#include <string>

#include "Listener.hpp"

class Client : public Listener {

  private:
	int fd;
	std::string name;

	void onPoll();

  public:
	Client(int fd);
	virtual ~Client();

	int getFd() const;
};
