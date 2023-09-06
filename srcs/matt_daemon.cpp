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

	/* Close all open file descriptors */
	for (int fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--)
		close(fd);

	return (EXIT_SUCCESS);
}

int			main(int argc, char *argv[]) {
	(void)argc;
	if (init_daemon()) {
		std::cerr << argv[0] << ": " << std::strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}
	Tintin_reporter tintin(LOG_FILE);
	tintin.log(LogLevel::Log, "Hello World!");
	while (1) {}
	return (EXIT_SUCCESS);
}
