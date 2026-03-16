#include "utils/logger.h"

#include <Arduino.h>

#include "config/app_config.h"

namespace xenovent::utils {

namespace {
LogLevel gLevel = LogLevel::Info;

const char* toString(LogLevel level) {
  switch (level) {
    case LogLevel::Debug:
      return "D";
    case LogLevel::Info:
      return "I";
    case LogLevel::Warn:
      return "W";
    case LogLevel::Error:
      return "E";
  }
  return "?";
}

void vlogf(LogLevel level, const char* tag, const char* fmt, va_list args) {
  if (static_cast<int>(level) < static_cast<int>(gLevel)) {
    return;
  }
  Serial.printf("[%s][%s] ", toString(level), tag);
  Serial.vprintf(fmt, args);
  Serial.println();
}
}  // namespace

void initLogger() { Serial.begin(config::app::kSerialBaud); }

void setLogLevel(LogLevel level) { gLevel = level; }

LogLevel getLogLevel() { return gLevel; }

void logf(LogLevel level, const char* tag, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vlogf(level, tag, fmt, args);
  va_end(args);
}

void debug(const char* tag, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vlogf(LogLevel::Debug, tag, fmt, args);
  va_end(args);
}

void info(const char* tag, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vlogf(LogLevel::Info, tag, fmt, args);
  va_end(args);
}

void warn(const char* tag, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vlogf(LogLevel::Warn, tag, fmt, args);
  va_end(args);
}

void error(const char* tag, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vlogf(LogLevel::Error, tag, fmt, args);
  va_end(args);
}

}  // namespace xenovent::utils
