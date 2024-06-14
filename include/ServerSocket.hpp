#pragma once

#include "SocketListener.hpp"

#include <string>

class ServerSocket: public SocketListener {
private:
	int fd;
	int port;
	std::string password;

    void onPoll();

public:
    ServerSocket();
    ~ServerSocket();

    void init(int port, const std::string& password);
    int getFd() const;
};
