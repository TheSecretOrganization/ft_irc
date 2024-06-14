#pragma once

#include "Listener.hpp"

class ServerListener: public Listener {
private:
	int fd;
	int port;

    void onPoll();

public:
    ServerListener();
    ~ServerListener();

    void init(int port);
    int getFd() const;
};
