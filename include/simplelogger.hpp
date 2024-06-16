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

#define SF_LOG_VERSION_INFO() SF_LOG_INFO("SimpleLogger v0.0.1")

namespace slog
{

class SimpleLogger
{
public:
    SimpleLogger() = default;
    ~SimpleLogger() = default;

    static std::shared_ptr<SimpleLogger> GlobalLogger();

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

/** Debug and assert macros */
#ifndef NDEBUG

/* Do not log assertions or debug info on release builds*/
#define SF_ASSERT(condition, message)                                                                                  \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        throw slog::LogException(message);                                                                             \
    }

#else // NDEBUG

#define SF_ASSERT(condition, message)

#endif // NDEBUG

#define SF_LOG_EXCEPTION(exception) slog::SimpleLogger::GlobalLogger()->exception(exception)

#ifdef SF_MIN_LOG_LEVEL

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
#if SF_MIN_LOG_LEVEL > 3

#ifndef NDEBUG

#define SF_LOG_DEBUG(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::DEBUG)

#else

#define SF_LOG_DEBUG(message)

#endif // NDEBUG

#else // SF_MIN_LOG_LEVEL == 0
#define SF_LOG_DEBUG(message)
#endif // SF_MIN_LOG_LEVEL == 0

#if SF_MIN_LOG_LEVEL > 2
#define SF_LOG_INFO(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::INFO)
#else
#define SF_LOG_INFO(message)
#endif // SF_MIN_LOG_LEVEL > 0

#if SF_MIN_LOG_LEVEL > 1
#define SF_LOG_WARNING(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::WARNING)
#else
#define SF_LOG_WARNING(message)
#endif // SF_MIN_LOG_LEVEL > 1

#if SF_MIN_LOG_LEVEL > 0
#define SF_LOG_ERROR(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::ERROR)
#else
#define SF_LOG_ERROR(message)
#endif // SF_MIN_LOG_LEVEL > 2

#if SF_MIN_LOG_LEVEL > -1
#define SF_LOG_FATAL(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::FATAL)
#else
#define SF_LOG_FATAL(message)
#endif // SF_MIN_LOG_LEVEL > 3

#else // SF_MIN_LOG_LEVEL

// Log level not set, so default to everything
#define SF_LOG_DEBUG(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::DEBUG)
#define SF_LOG_INFO(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::INFO)
#define SF_LOG_WARNING(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::WARNING)
#define SF_LOG_ERROR(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::ERROR)
#define SF_LOG_FATAL(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::FATAL)

#endif // SF_MIN_LOG_LEVEL
