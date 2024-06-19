#include "Server.hpp"
#include <csignal>
#include <cstdio>
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

int main() {
	if (register_action(SIGINT, NULL, &handle_singint) == -1 ||
		register_action(SIGQUIT, NULL, SIG_IGN) == -1)
		return -1;

	try {
		Server::getInstance().start(6663, "");
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
