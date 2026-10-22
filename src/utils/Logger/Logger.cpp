/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Logger class
*/

#include "Logger.hpp"
#include <iomanip>
#include <ctime>


static std::ostream VOID_STREAM(nullptr);


RType::Utils::Logger::Logger(std::ostream& output) noexcept
    : m_Output(&output)
    , m_BaseLevel(DEBUG)
{

}

auto RType::Utils::Logger::log(Level level) -> std::ostream&
{
    if (level < m_BaseLevel)
        return VOID_STREAM;

    auto time = std::time(nullptr);
    auto localtime = *std::localtime(&time);

    return *m_Output << std::put_time(&localtime, "%d-%m-%Y %H:%M:%S") << " "
                     << "[" << levelFormat(level) << levelName(level) << "\033[0m] "
                     << "- ";
}

auto RType::Utils::Logger::levelFormat(Level level) noexcept -> std::string_view
{
    switch (level) {
        case DEBUG:   return "\033[35;1m";  // magenta
        case INFO:    return "\033[34;1m";  // blue
        case WARNING: return "\033[33;1m";  // yellow-ish orange
        case ERROR:   return "\033[35;1m";  // red
        case FATAL:   return "\033[97;1m";  // bright white
        default:      return "";
    }
}

auto RType::Utils::Logger::levelName(Level level) noexcept -> std::string_view
{
    switch (level) {
        case DEBUG:   return "DEBUG";
        case INFO:    return "INFO";
        case WARNING: return "WARNING";
        case ERROR:   return "ERROR";
        case FATAL:   return "FATAL";
        default:      return "???";
    }
}
