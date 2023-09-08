#include "Matt_daemon.hpp"

Lock::Lock(std::string const &str) {
	this->str = str;
	std::string binding = str.substr(0, str.find_last_of("/")).c_str();
	const char *path = binding.c_str();
	if (ft_mkdir(path))
		throw std::runtime_error(std::strerror(errno));
	this->fd = open(str.c_str(), O_CREAT, 0644);
	if (this->fd < 0 || flock(this->fd, LOCK_EX | LOCK_NB) < 0) {
		close(this->fd);
		std::cerr << "Can't open: " << str << std::endl;
		throw std::runtime_error("Error file locked.");
	}
}

// Avoid to remove the file on parent exit
void	Lock::remove(void) {
	close(this->fd);
	this->fd = -1;
	std::remove(this->str.c_str());
}

Lock::~Lock(void) {
}

Lock::Lock(Lock const &l) {
	*this = l;
}

Lock		&Lock::operator=(Lock const &l) {
	this->fd = l.fd;
	return (*this);
}
