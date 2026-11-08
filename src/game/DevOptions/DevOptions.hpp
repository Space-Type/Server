/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** command line and developer options
*/

#pragma once

#include <cstdint>
#include <optional>
#include <string_view>


namespace RType::Game {
    /**
     * Cheats to test a level faster. Off unless the server
     * is started with them: clients can never turn them on.
     */
    struct DevOptions {
        bool god = false;           // players never lose a life
        float speed = 1.0F;         // the level scrolls this many times faster
        std::uint32_t start = 0;    // progression the level starts at
        std::optional<float> dropRate;  // chance (0 to 1) of every enemy dropping its bonus, whatever its stage file says

        [[ nodiscard ]] inline bool any() const noexcept { return god || speed != 1.0F || start != 0 || dropRate; }
    };

    struct CommandLine {
        bool help = false;
        DevOptions dev;
    };

    inline constexpr float DEV_SPEED_MAX = 50.0F;

    inline constexpr std::string_view USAGE =
        "USAGE: r-type_server [options]\n"
        "\n"
        "Runs the R-Type server on port 5432.\n"
        "\n"
        "Options:\n"
        "  --help           display this help and exit\n"
        "\n"
        "Developer options (to test a level, never on a real server):\n"
        "  --god            players never lose a life\n"
        "  --speed <n>      the level scrolls n times faster (0 < n <= 50)\n"
        "  --start <n>      the level starts at progression n (pixels of the stage background)\n"
        "  --drop-rate <p>  every enemy drops its bonus with chance p (0 <= p <= 1)\n";

    // Throws std::invalid_argument with a message for the user on an unknown option or a bad value
    CommandLine parseCommandLine(int argc, const char* const* argv);
}
