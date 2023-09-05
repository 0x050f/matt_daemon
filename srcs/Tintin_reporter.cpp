# include "matt_daemon.hpp"

std::ostream& operator<<(std::ostream& out, const LogLevel value){
	return out << [value]{
		switch(value){
			case Info: return "INFO";
			case Error: return "ERROR";
			case Log: return "LOG";
		}
	};
}

Tintin_reporter::Tintin_reporter(std::string const &path): file { path } {
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open log file");
	}
}

Tintin_reporter::~Tintin_reporter(void) {
	file.close();
}

Tintin_reporter::Tintin_reporter(const Tintin_reporter &t) {
	*this = t;
}

Tintin_reporter			&Tintin_reporter::operator=(const Tintin_reporter &t) {
	*this->file = t.file;
	return (*this);
}

template<typename T>
void					Tintin_reporter::log(const LogLevel l, T const& v)
{
	char time_string[100];

	std::time_t t = std::time(0);
	if (std::strftime(time_string, sizeof(time_string), "[%d/%m/%Y-%H:%M:%S]", std::localtime(&t)))
		file << time_string << " [ " << l << " ] " << v <<  std::endl;
}
