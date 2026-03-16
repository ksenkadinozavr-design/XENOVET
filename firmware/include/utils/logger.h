#pragma once

#include <cstdarg>

namespace xenovent::utils {

enum class LogLevel {
  Debug = 0,
  Info,
  Warn,
  Error,
};

void initLogger();
void setLogLevel(LogLevel level);
LogLevel getLogLevel();
void logf(LogLevel level, const char* tag, const char* fmt, ...);

void debug(const char* tag, const char* fmt, ...);
void info(const char* tag, const char* fmt, ...);
void warn(const char* tag, const char* fmt, ...);
void error(const char* tag, const char* fmt, ...);

}  // namespace xenovent::utils
