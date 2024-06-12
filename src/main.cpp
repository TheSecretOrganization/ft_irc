#include "Server.hpp"

int main() {
	Server server(6663, "");
	server.start();

	return 0;
}
