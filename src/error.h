#pragma once

#include <string>

[[noreturn]] void die(const std::string& message, int exit_code);
