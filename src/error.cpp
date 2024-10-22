#include "error.h"

#include <iostream>

[[noreturn]] void die(const std::string& message, int exit_code) {
    std::cerr << message << std::endl;
    std::exit(exit_code);
}
