#pragma once

#include <string>
#include <vector>

#include "Client.hpp"
#include "SocketPublisher.hpp"
#include "SocketSubscriber.hpp"

class Server: public SocketSubscriber {

private:
	int fd;
	int port;
	std::string password;
	std::vector<Client *> clients;
	SocketPublisher publisher;

	void onPoll();

public:
	Server(int port, const std::string &password);

	void start();
	void shut();

};
