#pragma once

#include "Socket.hpp"

class ClientSocket: public Socket {
private:
	int fd;

    void onPoll();

public:
    ClientSocket(int fd);
    ~ClientSocket();

    int getFd() const;
};
