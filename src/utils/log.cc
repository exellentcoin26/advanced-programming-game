#include "log.h"

std::shared_ptr<utils::Logger> utils::Logger::instance = 0;
const char* utils::Logger::log_level_strings[] = {"ERROR", "WARN", "INFO", "DEBUG"};

utils::LogStream::LogStream(bool ignore, std::ostream& os) { (ignore) ? this->os = 0 : this->os = &os; };
