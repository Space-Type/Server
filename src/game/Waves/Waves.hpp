/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** stage waves loaded from data files
*/

#pragma once

#include "../../components/Body/Body.hpp"
#include <protecs/networking.hpp>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>


namespace RType::Game {
    // One enemy of a stage, appearing on the right when the level reaches its progression
    struct Wave {
        std::uint32_t progression;  // background pixels (Level::progression)
        Protecs::Networking::Packets::EntityCreate::EntityType type;
        float y;                    // world units, top of the enemy
        Components::Pattern pattern;
    };

    /**
     * Reads a stage file: one enemy per line,
     *
     *     # progression  type       y    movement
     *     600            pata-pata  120  sine
     *
     * Types: pata-pata. Movements: straight, sine. Empty lines
     * and lines starting with '#' are skipped. An invalid line
     * is skipped too, and described in `errors` (with its
     * number): a broken file never stops the server. The
     * result is sorted by progression.
     */
    std::vector<Wave> parseWaves(std::string_view text, std::vector<std::string>& errors);

    /**
     * Finds `assets/stages/<name>` from the current directory or
     * one of its parents (the server runs from its repository or
     * its build directory) and parses it. Problems are logged;
     * no file or an unreadable one gives no wave.
     */
    std::vector<Wave> loadWaves(const std::string& name) noexcept;

    // A stage has a few hundred lines of ~30 bytes at most: anything bigger is not a stage file
    inline constexpr std::size_t WAVES_FILE_MAX = 64 * 1024;
}
