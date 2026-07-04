#pragma once

#include <iostream>
#include "log_level.hpp"

namespace idarpc {
    void log(LogLevel level,  const std::string& content);
}