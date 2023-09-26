#ifndef MATT_DAEMON_HPP
# define MATT_DAEMON_HPP

# include <ctime>
# include <csignal>
# include <cstring>
# include <mutex>
# include <iostream>

# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <sys/stat.h>

# include "Tintin_reporter.hpp"
# include "Lock.hpp"
# include "Server.hpp"

# define LOG_FILE "/var/log/matt_daemon/matt_daemon.log"
# define LOCK_FILE "/var/lock/matt_daemon.lock"
# define SERVER_PORT 4242

extern Tintin_reporter		*tintin;
extern bool					quit;
extern std::mutex			check_quit;

int		ft_mkdir(const char *dir);

#endif
