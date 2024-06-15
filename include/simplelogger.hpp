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

namespace sflog
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

} // namespace sflog

/** Debug and assert macros */
#ifndef NDEBUG

#define SF_ASSERT(condition, message)                                                                                  \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        throw sflog::LogException(message);                                                                            \
    }

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
#define SF_LOG_DEBUG(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::DEBUG)
#else // SF_MIN_LOG_LEVEL == 0
#define SF_LOG_DEBUG(message)
#endif // SF_MIN_LOG_LEVEL == 0

#if SF_MIN_LOG_LEVEL > 2
#define SF_LOG_INFO(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::INFO)
#else
#define SF_LOG_INFO(message)
#endif // SF_MIN_LOG_LEVEL > 0

#if SF_MIN_LOG_LEVEL > 1
#define SF_LOG_WARNING(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::WARNING)
#else
#define SF_LOG_WARNING(message)
#endif // SF_MIN_LOG_LEVEL > 1

#if SF_MIN_LOG_LEVEL > 0
#define SF_LOG_ERROR(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::ERROR)
#else
#define SF_LOG_ERROR(message)
#endif // SF_MIN_LOG_LEVEL > 2

#if SF_MIN_LOG_LEVEL > -1
#define SF_LOG_FATAL(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::FATAL)
#else
#define SF_LOG_FATAL(message)
#endif // SF_MIN_LOG_LEVEL > 3

#else // SF_MIN_LOG_LEVEL

// Log level not set, so default to everything
#define SF_LOG_DEBUG(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::DEBUG)
#define SF_LOG_INFO(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::INFO)
#define SF_LOG_WARNING(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::WARNING)
#define SF_LOG_ERROR(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::ERROR)
#define SF_LOG_FATAL(message) sflog::SimpleLogger::GlobalLogger()->log(message, sflog::LogLevel::FATAL)

#endif // SF_MIN_LOG_LEVEL

#else // NDEBUG

/* Do nothing for release builds */

#define SF_ASSERT(condition, message)

#define SF_LOG_DEBUG(message)
#define SF_LOG_INFO(message)
#define SF_LOG_WARNING(message)
#define SF_LOG_ERROR(message)
#define SF_LOG_FATAL(message)

#endif // NDEBUG
