#include "matt_daemon.hpp"

Lock::Lock(std::string const &str) {
	this->str = str;
	std::string binding = str.substr(0, str.find_last_of("/")).c_str();
	const char *path = binding.c_str();
	if (ft_mkdir(path))
		throw std::runtime_error(std::strerror(errno));
	this->fd = open(str.c_str(), O_CREAT, 0644);
	if (this->fd < 0)
		throw std::runtime_error(std::strerror(errno));
	if (flock(this->fd, LOCK_EX | LOCK_NB) < 0)
		throw std::runtime_error("Error file locked.");
}

Lock::~Lock(void) {
	close(this->fd);
	std::remove(this->str.c_str());
}

Lock::Lock(Lock const &l) {
	*this = l;
}

Lock		&Lock::operator=(Lock const &l) {
	this->fd = l.fd;
	return (*this);
}
