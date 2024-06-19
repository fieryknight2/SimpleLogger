/**
 * @brief Ability to log to console or files
 *
 * @author Matthew Brown
 * @date 6/15/2024
 */
#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "logexception.hpp"

namespace slog
{

/* Enums for logger information */
enum class LogLevel
{
    NONE = -1,
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

enum class LogFileMode
{
    APPEND,
    OVERWRITE
};

constexpr uint32_t MAX_LOG_LEVEL_NAME_LENGTH = 7;

std::string getLogName(LogLevel level);
std::string formatStringFromLeft(const std::string &name, uint32_t size);

/* Logger interface + sub classes */
class LoggerLoc
{
    /* You should be able to use this as a base class for your own loggers as well */
public:
    LoggerLoc() = default;
    virtual ~LoggerLoc() = default;

    virtual void log(const std::string &message, LogLevel level) = 0;
    virtual void exception(const LogException &exception) = 0;

    void setMaxLogLevel(const LogLevel level) { m_maxLogLevel = level; }
    void setMinLogLevel(const LogLevel level) { m_minLogLevel = level; }

protected:
    LogLevel m_maxLogLevel = LogLevel::FATAL;
    LogLevel m_minLogLevel = LogLevel::INFO; // Default to INFO

    [[nodiscard]] static std::string getTime();
};

class SimpleConsoleLogger final : public LoggerLoc
{
public:
    SimpleConsoleLogger() = default;

    void log(const std::string &message, LogLevel level) override;
    void exception(const LogException &exception) override;

    void enableColor() { m_color = true; }
    void enableColor(const bool enable) { m_color = enable; }
    void disableColor() { m_color = false; }
    [[nodiscard]] bool isColorEnabled() const { return m_color; }

private:
    bool m_color = false;
};

class ConsoleLogger final : public LoggerLoc
{
public:
    ConsoleLogger() = default;

    void log(const std::string &message, LogLevel level) override;
    void exception(const LogException &exception) override;

    [[nodiscard]] uint32_t getRepeatCount() const { return m_repeatCount; }

    void enableFullColor() { m_fullColor = true; }
    void enableFullColor(const bool enable) { m_fullColor = enable; }
    void disableFullColor() { m_fullColor = false; }
    [[nodiscard]] bool isFullColorEnabled() const { return m_fullColor; }

    void enableColor() { m_color = true; }
    void enableColor(const bool enable) { m_color = enable; }
    void disableColor() { m_color = false; }
    [[nodiscard]] bool isColorEnabled() const { return m_color; }

private:
    std::string m_repeatedMessage;
    /* To prevent accidental message repetition while different log levels */
    LogLevel m_repeatedLevel = LogLevel::NONE;

    bool m_color = false;
    bool m_fullColor = true;

    uint32_t m_repeatCount = 0;
};

class FileLogger final : public LoggerLoc
{
public:
    FileLogger() = default;
    explicit FileLogger(const std::string &filename);
    FileLogger(const std::string &filename, LogFileMode mode);
    ~FileLogger() override;

    void openFile(const std::string &filename) noexcept(false); /* Defaults to APPEND */
    void openFile(const std::string &filename, LogFileMode mode) noexcept(false);
    void closeFile();

    void log(const std::string &message, LogLevel level) override;
    void exception(const LogException &exception) override;

private:
    std::ofstream m_file;
};

} // namespace slog
