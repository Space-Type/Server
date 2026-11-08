/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** stages loaded from data files
*/

#pragma once

#include "../../components/Body/Body.hpp"
#include "../../components/Loot/Loot.hpp"
#include <protecs/networking.hpp>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace RType::Game {
    // One enemy of a stage, appearing on the right when the level reaches its progression
    struct Wave {
        std::uint32_t progression;  // background pixels (Level::progression)
        Protecs::Networking::Packets::EntityCreate::EntityType type;
        float y;                    // world units, top of the enemy
        bool randomY = false;       // `random` in the file: y is drawn when the enemy appears
        Components::Pattern pattern;
        std::optional<Components::Loot> loot;  // what it may drop when a player kills it (none: Blueprints::enemy's default)
    };

    // A stage: how long it scrolls, its enemies, and the boss at its end
    struct Stage {
        std::uint32_t length = 0;  // progression where the scrolling stops and the boss comes
        std::string boss;          // Blueprints::boss name; empty: the stage is cleared at its end
        std::vector<Wave> waves;   // sorted by progression
    };

    /**
     * Reads a stage file: its length, its boss, then one enemy
     * per line,
     *
     *     length 3797
     *     boss   dobkeratops
     *
     *     # progression  type       y       movement  loot
     *     600            pata-pata  120     sine
     *     660            pata-pata  random  straight  life
     *     720            pata-pata  200     sine      life:0.5
     *
     * y is `random` for a height drawn when the enemy appears
     * (Rules::RANDOM_Y_MIN to RANDOM_Y_MAX), different in every
     * game. Types: pata-pata, pow-armor. Movements: straight,
     * sine. The loot is optional: `life` or `force` always drops
     * that item, `life:<chance>` or `force:<chance>` with a chance
     * from 0 to 1, `none` nothing; without it, the enemy's own
     * (Blueprints::enemy). Bosses: see
     * Blueprints::isBoss. Empty lines and lines starting with
     * '#' are skipped. An invalid line is skipped too, and
     * described in `errors` (with its number): a broken file
     * never stops the server. Without a length, the stage
     * never ends.
     */
    Stage parseStage(std::string_view text, std::vector<std::string>& errors);

    /**
     * Finds `stages/<name>` from the current directory or
     * one of its parents (the server runs from its repository or
     * its build directory) and parses it. Problems are logged;
     * no file gives nothing, an unreadable one an empty stage.
     */
    std::optional<Stage> loadStage(const std::string& name) noexcept;

    // Loads stage-1.waves, stage-2.waves, ... up to the first missing one: stage n is at index n - 1
    std::vector<Stage> loadStages() noexcept;

    // A stage has a few hundred lines of ~30 bytes at most: anything bigger is not a stage file
    inline constexpr std::size_t WAVES_FILE_MAX = 64 * 1024;
}
