#include "Client.hpp"

Client::Client(int fd)
: fd(fd) { }

void Client::onPacket(const std::string &content) {

}
