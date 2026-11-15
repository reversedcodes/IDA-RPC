#pragma once

#include <string>
#include "ida_discord_rpc/util/log_level.hpp"

namespace idarpc {
    void log(LogLevel level,  const std::string& content);
}
