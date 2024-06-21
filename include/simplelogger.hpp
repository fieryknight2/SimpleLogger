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

/** Logs the version information for SimpleLogger */
#define SIMPLE_LOGGER_LOG_VERSION_INFO()                                                                               \
    SL_LOG_INFO("--- Using SimpleLogger " + std::string(slog::SimpleLoggerVersion) + " ---")
/** Logs version information in a certain format */
#define SL_LOG_VERSION_INFO(name, version)                                                                             \
    SL_LOG_INFO("--- " + std::string(name) + " v" + std::string(version) + " ---")

namespace slog
{

constexpr auto SimpleLoggerVersion = "v0.0.4";

/**
 * Main class for accessing information of SimpleLogger
 * For the most part you can just use the Prepocessor macros instead though
 */
class SimpleLogger
{
public:
    SimpleLogger() = default;
    ~SimpleLogger();

    /** Access to the global logger (singleton) */
    static std::shared_ptr<SimpleLogger> GlobalLogger();

    /** Enable capturing all uncaught exceptions (via std::uncaught_exception()) */
    static void CaptureExceptions();

    /** Log message with the provided level, options include slog::LogLevel::[DEBUG, INFO, WARNING, ERROR, FATAL] */
    void log(const std::string &message, LogLevel level);
    /** Log a slog::LogException, equivalent to log(exception.what(), slog::LogLevel::FATAL) for default loggers */
    void exception(const LogException &exception);

    /** Set the maximum log level for the global logger, options include slog::LogLevel::[DEBUG, INFO, WARNING, ERROR,
     * FATAL] */
    void setMaxLogLevel(LogLevel level);
    /** Set the minimum log level for the global logger, see setMaxLogLevel */
    void setMinLogLevel(LogLevel level);

    /** Access methods for MaxLogLevel */
    [[nodiscard]] LogLevel getMaxLogLevel() const { return m_maxLogLevel; }
    /** Access methods for MinLogLevel */
    [[nodiscard]] LogLevel getMinLogLevel() const { return m_minLogLevel; }

    /** Add a logger to the global logger, see slog::LoggerLoc */
    void addLogger(const std::shared_ptr<LoggerLoc> &loggerLoc);
    /** Remove a logger from the global logger, see slog::LoggerLoc */
    void removeLogger(const std::shared_ptr<LoggerLoc> &loggerLoc);
    /** Clear all loggers from the global logger, there's usually no reason to do this */
    void clearLoggers();

    /** Access a logger by index, returns nullptr if the index is out of bounds */
    std::shared_ptr<LoggerLoc> getLogger(uint32_t index);

private:
    /* The first logger is always the console logger */
    std::vector<std::shared_ptr<LoggerLoc>> m_loggerLocs;

    LogLevel m_maxLogLevel = LogLevel::FATAL;
    LogLevel m_minLogLevel = LogLevel::DEBUG;

    static std::shared_ptr<SimpleLogger> s_GlobalLogger;
};

} // namespace slog

#ifdef SL_ENABLE_STD_FORMAT
#include <format>
#endif // SL_ENABLE_STD_FORMAT

/** Gets the default console logger, this won't work if you've deleted it or cleared the loggers */
#define SL_GET_CONSOLE_LOGGER()                                                                                        \
    std::dynamic_pointer_cast<slog::ConsoleLogger>(slog::SimpleLogger::GlobalLogger()->getLogger(0))
/** Equivalent to slog::SimpleLogger::CaptureExceptions() */
#define SL_CAPTURE_EXCEPTIONS() slog::SimpleLogger::CaptureExceptions()
/** Log to a given file, possible modes include slog::LogFileMode::[APPEND, OVERWRITE] */
#define SL_LOG_TO_FILE(file, mode)                                                                                     \
    slog::SimpleLogger::GlobalLogger()->addLogger(std::make_shared<slog::FileLogger>(file, mode))

// Debug and assert macros
#ifndef NDEBUG

// Do not log assertions or debug info on release builds
/** Assertion that condition is false, on fail it will throw an slog::LogException with the message */
#define SL_ASSERT_FALSE(condition, message)                                                                            \
    if (condition)                                                                                                     \
    {                                                                                                                  \
        throw slog::LogException(message);                                                                             \
    }
/** Assertion that condition is true, on fail it will throw an slog::LogException with the message */
#define SL_ASSERT_TRUE(condition, message)                                                                             \
    if (!(condition))                                                                                                  \
    {                                                                                                                  \
        throw slog::LogException(message);                                                                             \
    }
/** Equivalent to SL_ASSERT_TRUE(condition, message) */
#define SL_ASSERT(condition, message) SL_ASSERT_TRUE(condition, message)

#else // NDEBUG

#define SL_ASSERT(condition, message)
#define SL_ASSERT_FALSE(condition, message)
#define SL_ASSERT_TRUE(condition, message)

#endif // NDEBUG

/** Log an exception where the message is e.what() and the log level is FATAL */
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

// NDEBUG forceably disables debug messages
#ifndef NDEBUG

#ifdef SL_ENABLE_STD_FORMAT
/** Log formatted message with the debug level */
#define SL_LOGF_DEBUG(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::DEBUG)
#endif // SL_ENABLE_STD_FORMAT

/** Log message with the debug level */
#define SL_LOG_DEBUG(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::DEBUG)

#else
#define SL_LOG_DEBUG(message)
#endif // NDEBUG

#else // SL_MIN_LOG_LEVEL == 0
#define SL_LOG_DEBUG(message)
#endif // SF_MIN_LOG_LEVEL == 0

#if SL_MIN_LOG_LEVEL > 2

#ifdef SL_ENABLE_STD_FORMAT
/** Log formatted message with the info level */
#define SF_LOGF_INFO(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::INFO)
#endif // SL_ENABLE_STD_FORMAT

/** Log message with the info level */
#define SF_LOG_INFO(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::INFO)

#else
#define SF_LOG_INFO(message)
#endif // SF_MIN_LOG_LEVEL > 0

#if SL_MIN_LOG_LEVEL > 1

#ifdef SL_ENABLE_STD_FORMAT
/** Log formatted message with the warning level */
#define Sl_LOGF_WARNING(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::WARNING)
#endif // SL_ENABLE_STD_FORMAT

/** Log message with the warning level */
#define SL_LOG_WARNING(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::WARNING)

#else
#define SL_LOG_WARNING(message)
#endif // SF_MIN_LOG_LEVEL > 1

#if SL_MIN_LOG_LEVEL > 0

#ifdef SL_ENABLE_STD_FORMAT
/** Log formatted message with the error level */
#define Sl_LOGF_ERROR(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::ERROR)
#endif // SL_ENABLE_STD_FORMAT

/** Log message with the error level */
#define Sl_LOG_ERROR(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::ERROR)

#else
#define SL_LOG_ERROR(message)
#endif // SF_MIN_LOG_LEVEL > 2

#if SL_MIN_LOG_LEVEL > -1

#ifdef SL_ENABLE_STD_FORMAT
/** Log formatted message with the fatal level */
#define Sl_LOGF_FATAL(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::FATAL)
#endif // SL_ENABLE_STD_FORMAT

/** Log message with the fatal level */
#define Sl_LOG_FATAL(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::FATAL)

#else
#define SL_LOG_FATAL(message)
#endif // SL_MIN_LOG_LEVEL > 3

#else // SL_MIN_LOG_LEVEL

#ifdef SL_ENABLE_STD_FORMAT

/** Log formatted message with the debug level */
#define SL_LOGF_DEBUG(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::DEBUG)
/** Log formatted message with the info level */
#define SL_LOGF_INFO(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::INFO)
/** Log formatted message with the warning level */
#define SL_LOGF_WARNING(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::WARNING)
/** Log formatted message with the error level */
#define SL_LOGF_ERROR(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::ERROR)
/** Log formatted message with the fatal level */
#define SL_LOGF_FATAL(...) slog::SimpleLogger::GlobalLogger()->log(std::format(__VA_ARGS__), slog::LogLevel::FATAL)

#endif // SL_ENABLE_STD_FORMAT
// Log level not set, so default to everything
/** Log message with the debug level */
#define SL_LOG_DEBUG(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::DEBUG)
/** Log message with the info level */
#define SL_LOG_INFO(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::INFO)
/** Log message with the warning level */
#define SL_LOG_WARNING(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::WARNING)
/** Log message with the error level */
#define SL_LOG_ERROR(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::ERROR)
/** Log message with the fatal level */
#define SL_LOG_FATAL(message) slog::SimpleLogger::GlobalLogger()->log(message, slog::LogLevel::FATAL)


#endif // SL_MIN_LOG_LEVEL
