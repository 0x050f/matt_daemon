#ifndef LOCK_HPP
# define LOCK_HPP

# include <sys/file.h>

class Lock {
	private:
		std::string		str;
		int				fd;

	public:
		Lock(std::string const &str);
		~Lock(void);
		Lock(Lock const &);
		Lock& operator=(Lock const &);
};

#endif
