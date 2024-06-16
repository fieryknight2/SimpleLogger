/**
 * @brief Simple logger for C++
 *
 * @author Matthew Brown
 * @date 6/15/2024
 */
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "loggerloc.hpp"

#define SIMPLE_LOGGER_LOG_VERSION_INFO() SL_LOG_INFO("--- Using SimpleLogger " + slog::SimpleLoggerVersion + " ---")
#define SL_LOG_VERSION_INFO(name, version)                                                                             \
    SL_LOG_INFO("--- " + std::string(name) + " v" + std::string(version) + " ---")

namespace slog
{

constexpr std::string SimpleLoggerVersion = "v0.0.2";

class SimpleLogger
{
public:
    SimpleLogger() = default;
    ~SimpleLogger() = default;

    static std::shared_ptr<SimpleLogger> GlobalLogger();

    static void CaptureExceptions();
    void log(const std::string &message, LogLevel level);
    void exception(const LogException &exception);

    void setMaxLogLevel(LogLevel level);
    void setMinLogLevel(LogLevel level);

    [[nodiscard]] LogLevel getMaxLogLevel() const { return m_maxLogLevel; }
    [[nodiscard]] LogLevel getMinLogLevel() const { return m_minLogLevel; }

    void addLogger(const std::shared_ptr<LoggerLoc> &loggerLoc);
    void removeLogger(const std::shared_ptr<LoggerLoc> &loggerLoc);
    void clearLoggers();

    std::shared_ptr<LoggerLoc> getLogger(uint32_t index);

private:
    /* The first logger is always the console logger */
    std::vector<std::shared_ptr<LoggerLoc>> m_loggerLocs;

    LogLevel m_maxLogLevel = LogLevel::FATAL;
    LogLevel m_minLogLevel = LogLevel::DEBUG;

    static std::shared_ptr<SimpleLogger> s_GlobalLogger;
};

} // namespace slog

#define SL_GET_CONSOLE_LOGGER()                                                                                        \
    std::dynamic_pointer_cast<slog::ConsoleLogger>(slog::SimpleLogger::GlobalLogger()->getLogger(0))
#define SL_CAPTURE_EXCEPTIONS() slog::SimpleLogger::CaptureExceptions()
#define SL_LOG_TO_FILE(file, mode)                                                                                     \
    slog::SimpleLogger::GlobalLogger()->addLogger(std::make_shared<slog::FileLogger>(file, mode))

/** Debug and assert macros */
#ifndef NDEBUG

/* Do not log assertions or debug info on release builds*/
#define SL_ASSERT(condition, message)                                                                                  \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        throw slog::LogException(message);                                                                             \
    }

#else // NDEBUG

#define SL_ASSERT(condition, message)

#endif // NDEBUG

#define SL_LOG_EXCEPTION(exp) slog::SimpleLogger::GlobalLogger()->exception(exp)

#ifdef SL_MIN_LOG_LEVEL

/* Logging macros
 *
 * 4: Debug
 * 3: Info
 * 2: Warning
 * 1: Error
 * 0: Fatal
 *
 * The number will also log every level below it,ex: 2 would log warning, error, and fatal
 */
#if SL_MIN_LOG_LEVEL > 3

#ifndef NDEBUG

#define SL_LOG_DEBUG(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::DEBUG)

#else

#define SL_LOG_DEBUG(message)

#endif // NDEBUG

#else // SL_MIN_LOG_LEVEL == 0
#define SL_LOG_DEBUG(message)
#endif // SF_MIN_LOG_LEVEL == 0

#if SL_MIN_LOG_LEVEL > 2
#define SF_LOG_INFO(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::INFO)
#else
#define SF_LOG_INFO(message)
#endif // SF_MIN_LOG_LEVEL > 0

#if SL_MIN_LOG_LEVEL > 1
#define SL_LOG_WARNING(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::WARNING)
#else
#define SL_LOG_WARNING(message)
#endif // SF_MIN_LOG_LEVEL > 1

#if SL_MIN_LOG_LEVEL > 0
#define SL_LOG_ERROR(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::ERROR)
#else
#define SL_LOG_ERROR(message)
#endif // SF_MIN_LOG_LEVEL > 2

#if SL_MIN_LOG_LEVEL > -1
#define Sl_LOG_FATAL(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::FATAL)
#else
#define SL_LOG_FATAL(message)
#endif // SL_MIN_LOG_LEVEL > 3

#else // SL_MIN_LOG_LEVEL

// Log level not set, so default to everything
#define SL_LOG_DEBUG(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::DEBUG)
#define SL_LOG_INFO(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::INFO)
#define SL_LOG_WARNING(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::WARNING)
#define SL_LOG_ERROR(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::ERROR)
#define SL_LOG_FATAL(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::FATAL)

#endif // SL_MIN_LOG_LEVEL
