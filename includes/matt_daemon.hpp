#ifndef MATT_DAEMON_HPP
# define MATT_DAEMON_HPP

# include <ctime>
# include <cstring>
# include <iostream>

# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <sys/stat.h>

# include "Tintin_reporter.hpp"
# include "Lock.hpp"
# include "Server.hpp"

# define LOG_FILE "/mnt/nfs/homes/lmartin/Documents/matt_daemon/log/matt_daemon/matt_daemon.log"
# define LOCK_FILE "/var/lock/matt_daemon.lock"
# define SERVER_PORT 14242

int		ft_mkdir(const char *dir);

#endif
