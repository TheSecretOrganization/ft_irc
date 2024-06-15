#include "Server.hpp"
#include <csignal>
#include <cstdio>
#include <iostream>

static void handle_singint(int signal) {
	(void)signal;
	Server::getInstance().shut();
}

static void handle_test(int signal) {
	(void) signal;
	std::string line;
	std::cout << "line: ";
	getline(std::cin, line);
	Client *client = Server::getInstance().getClients()[0];
	client->sendMessage("", line);
	std::cout << "send to " << client->getSocket().getFd() << ": " << line << std::endl;
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

int main() {
	register_action(SIGINT, NULL, &handle_singint);
	register_action(SIGQUIT, NULL, &handle_test);

	Server::getInstance().start(6663, "");

	return 0;
}
