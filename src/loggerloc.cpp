/* Created by Matthew Brown on 6/15/2024 */
#include "loggerloc.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>

constexpr auto DEBUG_COLOR = "\033[34m";
constexpr auto INFO_COLOR = "\033[32m";
constexpr auto WARNING_COLOR = "\033[33m";
constexpr auto ERROR_COLOR = "\033[31m";
constexpr auto FATAL_COLOR = "\033[41m";
constexpr auto RESET_COLOR = "\033[0m";

namespace slog
{

std::unordered_map<LogLevel, std::string> LogLevelColors = {
        {LogLevel::NONE, RESET_COLOR},      {LogLevel::DEBUG, DEBUG_COLOR}, {LogLevel::INFO, INFO_COLOR},
        {LogLevel::WARNING, WARNING_COLOR}, {LogLevel::ERROR, ERROR_COLOR}, {LogLevel::FATAL, FATAL_COLOR}};

std::unordered_map<LogLevel, std::string> LogLevelNames = {{LogLevel::NONE, "NONE"},   {LogLevel::DEBUG, "DEBUG"},
                                                           {LogLevel::INFO, "INFO"},   {LogLevel::WARNING, "WARNING"},
                                                           {LogLevel::ERROR, "ERROR"}, {LogLevel::FATAL, "FATAL"}};

std::string getLogName(const LogLevel level) { return LogLevelNames[level]; }

std::string formatStringFromLeft(const std::string &name, uint32_t size)
{
    std::string formattedName = name;

    if (name.size() < size)
    {
        formattedName = std::string(size - name.size(), ' ') + name;
    }

    return formattedName;
}

std::string LoggerLoc::getTime()
{
    static constexpr int MAX_DATE_LENGTH = 11;

    // Get the current time
    std::stringstream ss;
    const auto now = std::chrono::system_clock::now();
    auto t = now.time_since_epoch();

    // Get the current date
    const auto date = std::chrono::system_clock::to_time_t(now);
    char dateString[MAX_DATE_LENGTH];
    std::strftime(&dateString[0], MAX_DATE_LENGTH, "%d/%m/%Y", std::localtime(&date));

    ss << dateString << " ";

    ss << std::setfill('0') << (std::chrono::duration_cast<std::chrono::hours>(t) % 24).count();
    ss << ":";
    ss << std::setfill('0') << std::setw(2) << (std::chrono::duration_cast<std::chrono::minutes>(t) % 60).count();
    ss << ":";
    ss << std::setfill('0') << std::setw(2) << (std::chrono::duration_cast<std::chrono::seconds>(t) % 60).count();
    ss << ".";
    ss << std::setfill('0') << std::setw(3)
       << (std::chrono::duration_cast<std::chrono::milliseconds>(t) % 1000).count();

    return ss.str();
}

void SimpleConsoleLogger::log(const std::string &message, const LogLevel level)
{
    if (level < m_minLogLevel or level > m_maxLogLevel)
        return;

    std::stringstream out;

    if (m_color)
        out << LogLevelColors[level];

    out << std::endl;
    out << "[" << getTime() << " ";
    out << formatStringFromLeft(LogLevelNames[level], MAX_LOG_LEVEL_NAME_LENGTH);
    out << "]: ";

    out << message;
    out << "  "; // Some spacing

    if (m_color)
        out << RESET_COLOR;

    if (level < LogLevel::ERROR)
    {
        // Try to make sure output is properly flushed
        std::cerr << std::flush;
        std::cout << std::flush << out.str() << std::flush;
    }
    else
    {
        std::cout << std::flush;
        std::cerr << std::flush << out.str() << std::flush;
    }
}

void SimpleConsoleLogger::exception(const LogException &exception)
{
    std::string error = "Uncaught Exception Occurred! ";
    error += exception.what();

    log(error, LogLevel::FATAL);
}

void ConsoleLogger::log(const std::string &message, const LogLevel level)
{
    if (level < m_minLogLevel or level > m_maxLogLevel)
        return;

    if (m_repeatedLevel == level and m_repeatedMessage == message)
    {
        // Repeated message
        m_repeatCount++;

        std::cout << "\r"; // This should work

        if (m_fullColor)
            std::cout << LogLevelColors[level];

        std::cout << "[" << getTime() << " " << (m_color and !m_fullColor ? LogLevelColors[level] : "")
                  << formatStringFromLeft(LogLevelNames[level], MAX_LOG_LEVEL_NAME_LENGTH);
        std::cout << " (Rep: " << m_repeatCount << ")" << (m_color and !m_fullColor ? RESET_COLOR : "") << "]: ";
        std::cout << m_repeatedMessage;

        if (m_fullColor)
            std::cout << RESET_COLOR;

        std::cout << std::flush;

        return;
    }

    m_repeatCount = 1;
    m_repeatedMessage = message;
    m_repeatedLevel = level;

    std::stringstream out;

    if (m_fullColor)
        out << LogLevelColors[level];

    out << std::endl;
    out << "[" << getTime() << " " << (m_color and !m_fullColor ? LogLevelColors[level] : "");
    out << formatStringFromLeft(LogLevelNames[level], MAX_LOG_LEVEL_NAME_LENGTH);
    out << (m_color and !m_fullColor ? RESET_COLOR : "") << "]: ";

    out << message;
    out << "  "; // Some spacing

    if (m_fullColor)
        out << RESET_COLOR;

    if (level < LogLevel::ERROR)
    {
        std::cerr << std::flush;
        std::cout << std::flush << out.str() << std::flush;
    }
    else
    {
        std::cout << std::flush;
        std::cerr << std::flush << out.str() << std::flush;
    }
}

void ConsoleLogger::exception(const LogException &exception)
{
    std::string error = "Uncaught Exception Occurred! ";
    error += exception.what();

    log(error, LogLevel::FATAL);
}

FileLogger::FileLogger(const std::string &filename)
{
    openFile(filename);

    if (!m_file.is_open())
    {
        throw LogException("Could not open log file: " + filename);
    }
}

FileLogger::FileLogger(const std::string &filename, const LogFileMode mode) { openFile(filename, mode); }

FileLogger::~FileLogger()
{
    if (m_file.is_open())
        closeFile();
}

void FileLogger::openFile(const ::std::string &filename) { openFile(filename, LogFileMode::APPEND); }

void FileLogger::openFile(const std::string &filename, const LogFileMode mode)
{
    if (mode == LogFileMode::OVERWRITE)
    {
        m_file.open(filename, std::ios::out | std::ios::trunc);
    }
    else
    {
        m_file.open(filename, std::ios::out | std::ios::app);
    }

    if (!m_file.is_open())
    {
        throw LogException("Could not open log file: " + filename);
    }
}

void FileLogger::closeFile()
{
    if (m_file.is_open())
        m_file.close();
}

void FileLogger::log(const std::string &message, const LogLevel level)
{
    if (level < m_minLogLevel or level > m_maxLogLevel)
        return;

    m_file << "[" << getTime() << " " << formatStringFromLeft(LogLevelNames[level], MAX_LOG_LEVEL_NAME_LENGTH)
           << "]: " << message << std::endl;
    m_file.flush();
}

void FileLogger::exception(const LogException &exception)
{
    std::string error = "Uncaught Exception Occurred! ";
    error += exception.what();

    log(error, LogLevel::FATAL);
}


} // namespace slog
