#pragma once

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

const char* get_log_level_name(LogLevel level);