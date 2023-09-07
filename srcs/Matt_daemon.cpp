#include "Matt_daemon.hpp"

int		init_daemon(void) {
	pid_t	pid;

	/* Create a background process */
	pid = fork();

	/* An error occured */
	if (pid < 0)
		return (EXIT_FAILURE);

	/* Exit parent process gracefully */
	if (pid > 0)
		exit (EXIT_SUCCESS);

	/* Set new files permissions */
	umask(0);

	/* Child process becomes session leader and become detached from its tty */
	if (setsid() < 0)
		return (EXIT_FAILURE);

	/* Change the working directory to the root directory */
	chdir("/");

	/* Close file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return (EXIT_SUCCESS);
}

int		ft_mkdir(const char *dir) {
	char	tmp[PATH_MAX];
	char	*p = NULL;
	size_t	len;

	snprintf(tmp, sizeof(tmp),"%s",dir);
	len = strlen(tmp);
	if (tmp[len - 1] == '/')
		tmp[len - 1] = 0;
	for (p = tmp + 1; *p; p++)
		if (*p == '/') {
			*p = 0;
			if (mkdir(tmp, S_IRWXU) && errno != EEXIST)
				return (-1);
			*p = '/';
		}
	if (mkdir(tmp, S_IRWXU) && errno != EEXIST)
		return (-1);
	return (0);
}

Tintin_reporter		*tintin = NULL;
Lock				*lock = NULL;
Server				*server = NULL;

void		quit(int code) {
	tintin->log(LogLevel::Info, "Quitting.");
	delete(server);
	delete(lock);
	delete(tintin);
	exit(code);
}

void		signal_handler(int signum) {
	(void)signum;
	tintin->log(LogLevel::Info, "Signal handler.");
	quit(signum);
}

int			main(void) {
	char	buff[100];

	try {
		tintin = new Tintin_reporter(LOG_FILE);
		lock = new Lock(LOCK_FILE);
		tintin->log(LogLevel::Info, "Creating server.");
		server = new Server(SERVER_PORT);
		tintin->log(LogLevel::Info, "Server created.");
		tintin->log(LogLevel::Info, "Entering Daemon mode.");
		if (init_daemon())
			throw std::runtime_error(std::strerror(errno));
		if (std::snprintf(buff, 100, "started. PID: %d", getpid()) < 0)
			throw std::runtime_error(std::strerror(errno));
		tintin->log(LogLevel::Info, buff);
		for (size_t i = 0; i < _NSIG; i++)
			signal(i, signal_handler);
		server->run();
	} catch (const std::exception &e) {
		tintin->log(LogLevel::Error, e.what());
		quit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
