#include "Matt_daemon.hpp"

# define EXIT_PARENT 2

Tintin_reporter		*tintin = NULL;
bool				quit = false;
std::mutex			check_quit;

int		init_daemon(void) {
	pid_t	pid;

	/* Create a background process */
	pid = fork();

	/* An error occured */
	if (pid < 0)
		return (EXIT_FAILURE);

	/* Exit parent process gracefully */
	if (pid > 0) {
		return (EXIT_PARENT);
	}

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

void		signal_handler(int signum) {
	(void)signum;
	tintin->log(LogLevel::Info, "Signal handler.");
	check_quit.lock();
	quit = true;
	check_quit.unlock();
}

int			main(void) {
	int		ret;
	char	buff[100];

	ret = EXIT_SUCCESS;
	try {
		tintin = new Tintin_reporter(LOG_FILE);
		Lock lock(LOCK_FILE);
		tintin->log(LogLevel::Info, "Creating server.");
		Server server(SERVER_PORT);
		tintin->log(LogLevel::Info, "Server created.");
		tintin->log(LogLevel::Info, "Entering Daemon mode.");
		ret = init_daemon();
		if (ret == EXIT_FAILURE)
			throw std::runtime_error(std::strerror(errno));
		else if (ret == EXIT_PARENT) {
			delete(tintin);
			return (EXIT_SUCCESS);
		}
		if (std::snprintf(buff, 100, "started. PID: %d", getpid()) < 0)
			throw std::runtime_error(std::strerror(errno));
		tintin->log(LogLevel::Info, buff);
		for (size_t i = 0; i < _NSIG; i++)
			signal(i, signal_handler);
		server.run();
		lock.remove();
	} catch (const std::exception &e) {
		tintin->log(LogLevel::Error, e.what());
		ret = EXIT_FAILURE;
	}
	tintin->log(LogLevel::Info, "Quitting.");
	delete(tintin);
	return (ret);
}
