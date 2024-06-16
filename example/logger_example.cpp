/*
 * @brief Example of using the SimpleLogger library
 *
 * @author Matthew Brown
 * @date 6/15/2024
 */
#include <iostream>
#include <thread>
#include <unordered_map>

#include "simplelogger.hpp"

int main(const int argc, char *argv[])
{
    slog::SimpleLogger::GlobalLogger()->setMinLogLevel(slog::LogLevel::DEBUG);
    std::cout << "Log level: " << slog::getLogName(slog::SimpleLogger::GlobalLogger()->getMinLogLevel()) << std::endl;

    SIMPLE_LOGGER_LOG_VERSION_INFO();

    SL_LOG_WARNING("Ran using " + std::string(argv[0]));

    SL_LOG_DEBUG("This is a debug message");
    SL_LOG_DEBUG("This is a debug message");
    SL_LOG_DEBUG("This is a debug message");
    SL_LOG_DEBUG("This is a debug message");
    SL_LOG_INFO("This is an info message");
    SL_LOG_WARNING("This is a warning message");
    SL_LOG_ERROR("This is an error message");
    SL_LOG_FATAL("This is a fatal message");

    for (int i = 0; i < argc; i++)
    {
        SL_LOG_DEBUG("Argument: " + std::to_string(i) + " is " + std::string(argv[i]));
    }

    SL_LOG_INFO("Opening file example.log for logging");
    slog::SimpleLogger::GlobalLogger()->addLogger(std::make_shared<slog::FileLogger>("example.log"));

    SL_LOG_DEBUG("This is a debug message");
    for (int i = 0; i < 100; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        SL_LOG_DEBUG("This is a debug message");
    }

    SL_LOG_INFO("Enabling color");
    std::dynamic_pointer_cast<slog::ConsoleLogger>(slog::SimpleLogger::GlobalLogger()->getLogger(0))->enableColor(true);

    SL_LOG_INFO("This is an info message");
    SL_LOG_WARNING("This is a warning message");
    SL_LOG_ERROR("This is an error message");
    SL_LOG_FATAL("This is a fatal message");

    slog::SimpleLogger::GlobalLogger()->exception(slog::LogException("This is an exception"));

    try
    {
        SL_ASSERT(false, "This will fail.");
    }
    catch (const slog::LogException &exception)
    {
        slog::SimpleLogger::GlobalLogger()->exception(exception);
    }

    SL_ASSERT(false, "The end.");

    return 0;
}
