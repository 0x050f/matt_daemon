#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

# include <fstream>

enum LogLevel {
	Info,
	Error,
	Log,
};

class Tintin_reporter {
	private:
		std::ofstream file;

	public:
		Tintin_reporter(std::string const &path);
		~Tintin_reporter(void);
		Tintin_reporter(const Tintin_reporter &t);
		Tintin_reporter		&operator=(const Tintin_reporter &t);

		template<typename T>
		void	log(const LogLevel l, T const& v);
};

#endif
