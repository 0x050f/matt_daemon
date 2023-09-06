#include "matt_daemon.hpp"

Server::Server(uint16_t port) {
	struct sockaddr_in		addr;

	this->sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (this->sockfd < 0)
		throw std::runtime_error(std::strerror(errno));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
	if (bind(this->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(this->sockfd);
		throw std::runtime_error(std::strerror(errno));
	}
	if (listen(this->sockfd, 100) < 0) {
		close(this->sockfd);
		throw std::runtime_error(std::strerror(errno));
	}
	FD_ZERO(&this->fd_read);
	FD_ZERO(&this->fd_master);
	FD_SET(this->sockfd, &this->fd_master);
	this->fd_max = this->sockfd;
}

Server::~Server(void) {
	close(this->sockfd);
}

Server::Server(Server const &s) {
	*this = s;
}

Server		&Server::operator=(Server const &s) {
	this->sockfd = s.sockfd;
	this->fd_max = s.fd_max;
	this->fd_read = s.fd_read;
	this->fd_master = s.fd_master;
	this->clients = s.clients;
	return (*this);
}
