#include "Server.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>

static void handle_singint(int signal) {
	(void)signal;
	Server::getInstance().shut();
}

static int register_action(int signal, struct sigaction* old,
						   void (*handler)(int)) {
	struct sigaction saction;
	sigset_t set;

	sigemptyset(&set);
	saction.sa_handler = handler;
	saction.sa_mask = set;
	saction.sa_flags = 0;
	if (sigaction(signal, &saction, old) == -1) {
		std::cerr << "cannot create signal action" << std::endl;
		return -1;
	}
	return 0;
}

int serverParameters(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv [port] [password]" << std::endl;
		return -1;
	}

	int port = std::atoi(argv[1]);
	if (port < 6660 || port > 6669) {
		std::cerr << "Error: [port] must be between 6660 and 6669" << std::endl;
		return -1;
	}
	return port;
}

int main(int argc, char* argv[]) {
	int port = serverParameters(argc, argv);
	if (port == -1) {
		return -1;
	}

	if (register_action(SIGINT, NULL, &handle_singint) == -1 ||
		register_action(SIGQUIT, NULL, SIG_IGN) == -1)
		return -1;

	try {
		Server::getInstance().start(port, argv[2]);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
