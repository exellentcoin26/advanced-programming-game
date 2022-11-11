#include "utils/log.h"

#include <iostream>
#include <memory>

int main() {
    auto log_instance = utils::Logger::get_instance();
    log_instance->config(utils::Logger::LogLevel::Debug, std::cout);

    return 0;
}
