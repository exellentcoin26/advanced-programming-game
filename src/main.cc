#include "utils/log.h"

#include <iostream>

#include <dbg.h>

int main() {
    auto log_instance = utils::Logger::get_instance();
    log_instance->config(utils::Logger::LogLevel::Debug, std::cout);

    log(Debug) << "Hello, World\n";

    return 0;
}
