#include "Server.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>

static void handle_singint(int signal) {
	(void)signal;
	Server::getInstance().shut();
}

static void register_action(int signal, struct sigaction* old,
							void (*handler)(int)) {
	struct sigaction saction;
	sigset_t set;

	sigemptyset(&set);
	saction.sa_handler = handler;
	saction.sa_mask = set;
	saction.sa_flags = 0;
	if (sigaction(signal, &saction, old) == -1) {
		perror("sigaction");
		return;
	}
}

int serverParameters(int argc, char* argv[]) {
	int port;
	try {
		if (argc != 3) {
			throw Server::InvalidArgumentNumberException();
		}

		port = std::atoi(argv[1]);
		if (port < 6660 || port > 6669) {
			throw Server::InvalidPortRangeException();
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return (0);
	}

	std::cout << argv[2] << std::endl;
	return port;
}

int main(int argc, char* argv[]) {
	int port;
	port = serverParameters(argc, argv);
	if (!port) {
		return (1);
	}

	register_action(SIGINT, NULL, &handle_singint);
	register_action(SIGQUIT, NULL, SIG_IGN);

	Server::getInstance().start(port, argv[2]);

	return 0;
}
