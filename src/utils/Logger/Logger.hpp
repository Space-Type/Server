/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Logger class
*/

#pragma once

#include <iostream>


namespace RType::Utils {
    class Logger {
        public:
            enum Level {
                DEBUG = 0,
                INFO,
                WARNING,
                ERROR,
                FATAL,
            };

        private:
            std::ostream* m_Output;
            Level m_BaseLevel;

        public:
            Logger(std::ostream& output = std::cout) noexcept;

            inline void setLevel(Level level) noexcept { m_BaseLevel = level; }
            inline void setOutput(std::ostream& stream) noexcept { m_Output = &stream; }

            inline std::ostream& debug()   { return log(DEBUG); }
            inline std::ostream& info()    { return log(INFO); }
            inline std::ostream& warning() { return log(WARNING); }
            inline std::ostream& error()   { return log(ERROR); }
            inline std::ostream& fatal()   { return log(FATAL); }

            std::ostream& log(Level level);

        private:
            static std::string_view levelFormat(Level level) noexcept;
            static std::string_view levelName(Level level) noexcept;
    };
}

namespace RType {
    inline Utils::Logger logger;
}
