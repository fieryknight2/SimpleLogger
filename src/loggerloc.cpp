/* Created by Matthew Brown on 6/15/2024 */
#include "loggerloc.hpp"

#include <ctime>
#include <iostream>
#include <sstream>
#include <unordered_map>

constexpr std::string DEBUG_COLOR = "\033[34m;";
constexpr std::string INFO_COLOR = "\033[32m;";
constexpr std::string WARNING_COLOR = "\033[33m;";
constexpr std::string ERROR_COLOR = "\033[31m;";
constexpr std::string FATAL_COLOR = "\033[41m;";
constexpr std::string RESET_COLOR = "\033[0m;";

namespace slog
{

std::unordered_map<LogLevel, std::string> LogLevelColors = {
        {LogLevel::NONE, ""},           {LogLevel::DEBUG, DEBUG_COLOR},
        {LogLevel::INFO, INFO_COLOR},   {LogLevel::WARNING, WARNING_COLOR},
        {LogLevel::ERROR, ERROR_COLOR}, {LogLevel::FATAL, FATAL_COLOR}};

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
    const std::time_t time = std::time(nullptr);
    const std::tm *timeinfo = std::localtime(&time);

    char buffer[21];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);

    return buffer;
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

        std::cout << "[" << getTime() << " " << (m_color ? LogLevelColors[level] : "")
                  << formatStringFromLeft(LogLevelNames[level], MAX_LOG_LEVEL_NAME_LENGTH);
        std::cout << " (Rep: " << m_repeatCount << ")" << (m_color ? RESET_COLOR : "") << "]: ";
        std::cout << m_repeatedMessage;
        std::cout << std::flush;

        return;
    }

    if (m_repeatCount > 0) // Move to the next line
        std::cout << std::endl;

    m_repeatCount = 0;
    m_repeatedMessage = message;
    m_repeatedLevel = level;

    std::stringstream out;

    out << "[" << getTime() << " " << (m_color ? LogLevelColors[level] : "");
    out << formatStringFromLeft(LogLevelNames[level], MAX_LOG_LEVEL_NAME_LENGTH);
    out << (m_color ? RESET_COLOR : "") << "]: ";

    out << message;

    if (level < LogLevel::ERROR)
    {
        // Try to make sure output is properly flushed
        std::cerr << std::flush;
        std::cout << std::flush << out.str() << std::endl;
    }
    else
    {
        std::cout << std::flush;
        std::cerr << std::flush << out.str() << std::endl;
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
}

void FileLogger::exception(const LogException &exception)
{
    std::string error = "Uncaught Exception Occurred! ";
    error += exception.what();

    log(error, LogLevel::FATAL);
}


} // namespace slog
