# include "matt_daemon.hpp"

static int		_mkdir(const char *dir) {
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

Tintin_reporter::Tintin_reporter(std::string const &str) {
	std::string binding = str.substr(0, str.find_last_of("/")).c_str();
	const char *path = binding.c_str();
	if (_mkdir(path))
		throw std::runtime_error(std::strerror(errno));
	this->file = std::ofstream(str);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open log file");
	}
}

Tintin_reporter::~Tintin_reporter(void) {
	file.close();
}
