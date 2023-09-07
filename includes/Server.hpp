#ifndef SERVER_HPP
# define SERVER_HPP

# include <arpa/inet.h>
# include <vector>

# define MAX_CLIENTS 3

class Server {
	private:
		int					sockfd;
		int					fd_max;
		fd_set				fd_read;
		fd_set				fd_master;
		std::vector<int>	clients;

		void	new_client();
		void	remove_client(int fd);
		void	recvfrom_client(int fd);

	public:
		Server(uint16_t port);
		~Server(void);
		Server(Server const &);
		Server& operator=(Server const &);
		void	run(void);
};

#endif
