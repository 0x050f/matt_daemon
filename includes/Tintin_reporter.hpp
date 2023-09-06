#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

# include <filesystem>
# include <fstream>
# include <linux/limits.h>

enum LogLevel {
	Info,
	Error,
	Log,
};

inline std::ostream& operator<<(std::ostream& out, const LogLevel &value){
	switch(value){
		case Info: return out << "INFO";
		case Error: return out << "ERROR";
		case Log: return out << "LOG";
	}
	return out;
};

class Tintin_reporter {
	private:
		std::ofstream file;

	public:
		Tintin_reporter(std::string const &);
		~Tintin_reporter(void);
		/* Disable copy constructor  since std::ofstream is not copyable */
		Tintin_reporter(Tintin_reporter const&) = delete;
		/* Disable copy */
		Tintin_reporter& operator=(Tintin_reporter const&) = delete;

		template<typename T>
		void	log(const LogLevel l, T const& v)
		{
			char time_string[100];

			std::time_t t = std::time(0);
			if (std::strftime(time_string, sizeof(time_string), "[%d/%m/%Y-%H:%M:%S]", std::localtime(&t)))
				file << time_string << " [ " << l << " ] - Matt_daemon: " << v <<  std::endl;
		}

		template<typename Arg, typename ...Args>
		void	log(const LogLevel l, Arg const& arg, Args const&... args) {
			log(l, arg);
			log(l, args...);
		}
};

#endif
