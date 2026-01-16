//simulant of logging system

#include "log.h"

void logEvent(const std::string& event) {
	// In a real application, this would write to a log file or logging system
	std::cout << "[LOG]: " << event << std::endl;
}