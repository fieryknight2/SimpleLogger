/**
 * @brief Simple logger for C++
 *
 * @author Matthew Brown
 * @date 6/15/2024
 */
#pragma once

#include <exception>
#include <string>
#include <utility>

namespace sflog
{

class LogException : public std::exception
{
public:
    explicit LogException(std::string message) : m_message(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

} // namespace sflog
