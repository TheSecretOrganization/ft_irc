#pragma once

#include <string>
#include <vector>

#include "Client.hpp"
#include "SocketListener.hpp"
#include "SocketObserver.hpp"

class Server : public SocketListener {

  private:
	int fd;
	int port;
	std::string password;
	std::vector<Client*> clients;
	SocketObserver observer;

	void onPoll();

  public:
	Server(int port, const std::string& password);

	void start();
	void shut();
};
