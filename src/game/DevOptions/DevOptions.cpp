/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** command line and developer options
*/

#include "DevOptions.hpp"
#include <charconv>
#include <stdexcept>
#include <string>


namespace {
    template <typename T>
    T parseValue(std::string_view option, int argc, const char* const* argv, int& i)
    {
        if (i + 1 >= argc)
            throw std::invalid_argument("missing value for " + std::string(option));

        std::string_view text = argv[++i];
        T value{};
        auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);

        if (error != std::errc() || end != text.data() + text.size())
            throw std::invalid_argument("invalid value for " + std::string(option) + ": \"" + std::string(text) + "\"");
        return value;
    }
}


auto RType::Game::parseCommandLine(int argc, const char* const* argv) -> CommandLine
{
    CommandLine line;

    for (int i = 1; i < argc; ++i) {
        std::string_view option = argv[i];

        if (option == "--help") {
            line.help = true;
        } else if (option == "--god") {
            line.dev.god = true;
        } else if (option == "--speed") {
            line.dev.speed = parseValue<float>(option, argc, argv, i);
            if (!(line.dev.speed > 0.0F && line.dev.speed <= DEV_SPEED_MAX))
                throw std::invalid_argument("--speed must be above 0 and at most 50");
        } else if (option == "--start") {
            line.dev.start = parseValue<std::uint32_t>(option, argc, argv, i);
        } else if (option == "--drop-rate") {
            line.dev.dropRate = parseValue<float>(option, argc, argv, i);
            if (!(*line.dev.dropRate >= 0.0F && *line.dev.dropRate <= 1.0F))
                throw std::invalid_argument("--drop-rate must be from 0 to 1");
        } else {
            throw std::invalid_argument("unknown option: " + std::string(option));
        }
    }
    return line;
}
