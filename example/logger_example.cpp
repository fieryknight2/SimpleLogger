/*
 * @brief Example of using the SimpleLogger library
 *
 * @author Matthew Brown
 * @date 6/15/2024
 */
#include <iostream>
#include <thread>

#include "simplelogger.hpp"

int main(const int argc, char *argv[])
{
    slog::SimpleLogger::GlobalLogger()->setMinLogLevel(slog::LogLevel::DEBUG);
    SL_CAPTURE_EXCEPTIONS();
    SL_GET_CONSOLE_LOGGER()->enableColor(false);
    std::cout << "Log level: " << slog::getLogName(slog::SimpleLogger::GlobalLogger()->getMinLogLevel()) << std::endl;

    SIMPLE_LOGGER_LOG_VERSION_INFO();
    SL_LOG_VERSION_INFO("SimpleLogger Example", "1.0.0");

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
    SL_LOG_TO_FILE("example.log", slog::LogFileMode::APPEND);

    SL_LOG_INFO("Opening debug file example_debug.log for logging");
    SL_LOG_TO_FILE("example_debug.log", slog::LogFileMode::OVERWRITE);
    slog::SimpleLogger::GlobalLogger()->getLogger(2)->setMinLogLevel(slog::LogLevel::DEBUG);

    SL_LOG_DEBUG("This is a debug message");
    for (int i = 0; i < 100; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        SL_LOG_DEBUG("This is a debug message");
    }

    SL_LOG_INFO("Enabling color");
    SL_GET_CONSOLE_LOGGER()->enableColor(true);

    SL_LOG_INFO("This is an info message");
    SL_LOG_WARNING("This is a warning message");
    SL_LOG_ERROR("This is an error message");
    SL_LOG_FATAL("This is a fatal message");

    SL_LOG_EXCEPTION(slog::LogException("This is an exception"));

    // SL_ASSERT(false, "This will fail.");

    try
    {
        SL_ASSERT(false, "This will fail.");
    }
    catch (const slog::LogException &exception)
    {
        SL_LOG_EXCEPTION(exception);
    }

    SL_LOG_INFO("Disabling full colors");
    SL_GET_CONSOLE_LOGGER()->enableFullColor(false);

    SL_LOG_INFO("This is an info message");
    SL_LOG_WARNING("This is a warning message");
    SL_LOG_ERROR("This is an error message");
    SL_LOG_FATAL("This is a fatal message");

    for (int i = 0; i < 200; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        SL_LOG_INFO("This is a debug message");
    }

    SL_LOG_INFO("Re-enabling full colors");
    SL_GET_CONSOLE_LOGGER()->enableFullColor(true);

    for (int i = 0; i < 1000; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        SL_LOG_DEBUG("This is a debug message");
    }

    return 0;
}
