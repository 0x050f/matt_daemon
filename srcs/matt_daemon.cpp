#include "matt_daemon.hpp"

int		init_daemon(void) {
	pid_t	pid;

	/* Create a background process */
	pid = fork();

	/* An error occured */
	if (pid < 0) {
		return (EXIT_FAILURE);
	}

	/* Exit parent process gracefully */
	if (pid > 0)
		exit (EXIT_SUCCESS);

	/* Set new files permissions */
	umask(0);

	/* Child process becomes session leader and become detached from its tty */
	if (setsid() < 0) {
		return (EXIT_FAILURE);
	}

	/* Change the working directory to the root directory */
	chdir("/");

	/* Close file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return (EXIT_SUCCESS);
}

int			main(int argc, char *argv[]) {
	(void)argc;
	Tintin_reporter tintin(LOG_FILE);
	tintin.log(LogLevel::Info, "Started.");
	tintin.log(LogLevel::Info, "Creating server.");
	Server server(SERVER_PORT);
	tintin.log(LogLevel::Info, "Server created.");
	tintin.log(LogLevel::Info, "Entering Daemon mode.");
	if (init_daemon()) {
		std::cerr << argv[0] << ": " << std::strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}
	tintin.log(LogLevel::Info, "started. PID: ", getpid());
	while (1) {}
	return (EXIT_SUCCESS);
}
