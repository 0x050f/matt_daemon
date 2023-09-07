# include "matt_daemon.hpp"

Tintin_reporter::Tintin_reporter(std::string const &str) {
	std::string binding = str.substr(0, str.find_last_of("/")).c_str();
	const char *path = binding.c_str();
	if (ft_mkdir(path))
		throw std::runtime_error(std::strerror(errno));
	this->file = std::ofstream(str, std::ios_base::app);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open log file");
	}
	log(LogLevel::Info, "Started.");
}

Tintin_reporter::~Tintin_reporter(void) {
	log(LogLevel::Info, "Quitting.");
	file.close();
}
