#include "Matt_daemon.hpp"

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
	for (
		std::vector<int>::iterator iter = this->clients.begin();
		iter != this->clients.end();
		iter++
	) {
		FD_CLR(*iter, &this->fd_master);
		close(*iter);
	}
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

void		Server::new_client(void) {
	struct sockaddr_in		cli;
	socklen_t	len = sizeof(cli);
	int new_fd = accept(this->sockfd, (struct sockaddr *)&cli, &len);
	if (this->clients.size() < MAX_CLIENTS)
	{
		if (new_fd > this->fd_max)
			this->fd_max = new_fd;
		FD_SET(new_fd, &this->fd_master);
		this->clients.push_back(new_fd);
	}
	else
		close(new_fd);
}

void		Server::remove_client(int fd) {
	for (
		std::vector<int>::iterator iter = this->clients.begin();
		iter != this->clients.end();
		iter++
	) {
		if (*iter == fd) {
			FD_CLR(fd, &this->fd_master);
			close(*iter);
			this->clients.erase(iter);
			break;
		}
	}
}

void	Server::recvfrom_client(int fd) {
	int		len_read;
	char	buffer[BUFFER_SIZE + 1];

	bzero(&buffer, BUFFER_SIZE + 1);
	if ((len_read = recv(fd, buffer, BUFFER_SIZE, 0)) <= 0) // ERROR OR LEAVING
		remove_client(fd);
	else
	{
		char *msg = buffer;
		char *end = static_cast<char *>(memchr(buffer, '\n', len_read));
		if (!end)
			end = buffer + len_read;
		*end = '\0';
		while (msg - buffer < len_read)
		{
			char	format[BUFFER_SIZE + 32];

			if (!std::strcmp(msg, "quit")) {
				tintin->log(LogLevel::Info, "Request quit.");
				check_quit.lock();
				quit = true;
				check_quit.unlock();
				return ;
			}
			if (std::snprintf(format, BUFFER_SIZE + 32, "User input: %s", msg) < 0)
				return ;
			tintin->log(LogLevel::Log, format);
			msg += strlen(msg) + 1;
			end = static_cast<char *>(memchr(end + 1, '\n', len_read - (end - buffer)));
			if (!end)
				end = buffer + len_read;
			*end = '\0';
		}
	}
}

void		Server::run(void) {
	struct timeval tv;

	check_quit.lock();
	while (!quit) {
		check_quit.unlock();
		this->fd_read = this->fd_master;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		select(this->fd_max + 1, &this->fd_read, NULL, NULL, &tv);
		for (int fd = 0; fd < this->fd_max + 1 && !quit; fd++) {
			if (FD_ISSET(fd, &this->fd_read)) {
				if (fd == this->sockfd) {
					new_client();
					break ;
				} else {
					recvfrom_client(fd);
					break ;
				}
			}
		}
		check_quit.lock();
	}
	check_quit.unlock();
}
