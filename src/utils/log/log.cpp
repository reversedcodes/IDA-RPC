#include "log.hpp"
#include <idp.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

void idarpc::log(LogLevel level, const std::string &content)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream time_ss;
    time_ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");

    const char *level_name = get_log_level_name(level);

    msg("[IDARPC] [%s] [%s] %s\n", time_ss.str().c_str(), level_name, content.c_str());
}