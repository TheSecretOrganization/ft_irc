#pragma once

#include "SocketListener.hpp"

class ServerSocket: public SocketListener {
private:
	int fd;
	int port;

    void onPoll();

public:
    ServerSocket();
    ~ServerSocket();

    void init(int port);
    int getFd() const;
};
