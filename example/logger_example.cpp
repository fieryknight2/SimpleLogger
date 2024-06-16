/*
 * @brief Example of using the SimpleLogger library
 *
 * @author Matthew Brown
 * @date 6/15/2024
 */
#include <iostream>
#include <unordered_map>

#include "simplelogger.hpp"

int main(const int argc, char *argv[])
{
    slog::SimpleLogger::GlobalLogger()->setMinLogLevel(slog::LogLevel::DEBUG);
    std::cout << "Log level: " << slog::getLogName(slog::SimpleLogger::GlobalLogger()->getMinLogLevel()) << std::endl;

    SF_LOG_WARNING("Ran using " + std::string(argv[0]));

    SF_LOG_DEBUG("This is a debug message");
    SF_LOG_DEBUG("This is a debug message");
    SF_LOG_DEBUG("This is a debug message");
    SF_LOG_DEBUG("This is a debug message");
    SF_LOG_INFO("This is an info message");
    SF_LOG_WARNING("This is a warning message");
    SF_LOG_ERROR("This is an error message");
    SF_LOG_FATAL("This is a fatal message");

    for (int i = 0; i < argc; i++)
    {
        SF_LOG_DEBUG("Argument: " + std::string(argv[i] + std::string(" ") + "is " + std::string(argv[i])));
    }

    SF_LOG_INFO("Opening file example.log for logging");
    slog::SimpleLogger::GlobalLogger()->addLogger(std::make_shared<slog::FileLogger>("example.log"));
    SF_LOG_DEBUG("This is a debug message");

    SF_LOG_DEBUG("This is a debug message");
    SF_LOG_DEBUG("This is a debug message");
    SF_LOG_DEBUG("This is a debug message");
    SF_LOG_INFO("This is an info message");
    SF_LOG_WARNING("This is a warning message");
    SF_LOG_ERROR("This is an error message");
    SF_LOG_FATAL("This is a fatal message");

    slog::SimpleLogger::GlobalLogger()->exception(slog::LogException("This is an exception"));

    try
    {
        SF_ASSERT(false, "This will fail.");
    }
    catch (const slog::LogException &exception)
    {
        slog::SimpleLogger::GlobalLogger()->exception(exception);
    }

    SF_ASSERT(false, "The end.");

    return 0;
}
