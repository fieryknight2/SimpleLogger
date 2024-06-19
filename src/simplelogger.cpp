/* Created by Matthew Brown on 6/15/2024 */
#include "simplelogger.hpp"

#include <algorithm>
#include <iostream>

namespace slog
{

SimpleLogger::~SimpleLogger()
{
    clearLoggers();

    std::cout << "\n\n" << std::endl;
}

std::shared_ptr<SimpleLogger> SimpleLogger::s_GlobalLogger;

std::shared_ptr<SimpleLogger> SimpleLogger::GlobalLogger()
{
    if (!s_GlobalLogger)
    {
        s_GlobalLogger = std::make_unique<SimpleLogger>();

        s_GlobalLogger->addLogger(std::make_shared<ConsoleLogger>());
        /* Make sure the default logger will log everything */
        s_GlobalLogger->getLogger(0)->setMinLogLevel(LogLevel::DEBUG);
    }

    return s_GlobalLogger;
}

void SimpleLogger::CaptureExceptions()
{
    std::set_terminate(
            []()
            {
                try
                {
                    std::rethrow_exception(std::current_exception());
                }
                catch (const std::exception &e)
                {
                    SL_LOG_FATAL("Unhandled exception: " + std::string(e.what()));
                }

                std::abort();
            });
}

void SimpleLogger::log(const std::string &message, const LogLevel level)
{
    if (level < m_minLogLevel or level > m_maxLogLevel)
        return;

    // Log to all loggers (in order)
    for (auto &loggerLoc: m_loggerLocs)
    {
        if (loggerLoc != nullptr)
        {
            loggerLoc->log(message, level);
        }
    }
}

void SimpleLogger::exception(const LogException &exception)
{
    /* Exceptions have a log level of FATAL, maybe change to ERROR in the future... */
    if (m_maxLogLevel < LogLevel::FATAL)
        return;

    // Log to all loggers (in order)
    for (auto &loggerLoc: m_loggerLocs)
    {
        if (loggerLoc != nullptr)
        {
            loggerLoc->exception(exception);
        }
    }
}

void SimpleLogger::setMaxLogLevel(const LogLevel level) { m_maxLogLevel = level; }

void SimpleLogger::setMinLogLevel(const LogLevel level) { m_minLogLevel = level; }

void SimpleLogger::addLogger(const std::shared_ptr<LoggerLoc> &loggerLoc)
{
    if (loggerLoc == nullptr)
        return;

    m_loggerLocs.push_back(loggerLoc);
}

void SimpleLogger::removeLogger(const std::shared_ptr<LoggerLoc> &loggerLoc)
{
    if (loggerLoc == nullptr)
        return;

    if (const auto logger = std::ranges::find(m_loggerLocs, loggerLoc); logger != m_loggerLocs.end())
        m_loggerLocs.erase(logger);
}

void SimpleLogger::clearLoggers() { m_loggerLocs.clear(); }

std::shared_ptr<LoggerLoc> SimpleLogger::getLogger(const uint32_t index)
{
    if (index >= m_loggerLocs.size())
        return nullptr;

    return m_loggerLocs[index];
}

} // namespace slog
