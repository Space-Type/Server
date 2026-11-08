/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** enemy and boss blueprints
*/

#pragma once

#include "../Requests/Requests.hpp"
#include "../Waves/Waves.hpp"
#include <optional>
#include <string_view>


/**
 * What an enemy or a boss is made of: its spawn request,
 * with the behaviour components the generic systems run
 * (RandomShots, AimedBursts, Armored, Boss, ...). A new
 * enemy or boss is a new blueprint here.
 */
namespace RType::Game::Blueprints {
    // A stage file enemy, appearing on the right edge; `cooldown`: seconds before its first shot
    SpawnRequest enemy(const Wave& wave, float cooldown);

    // Names of the stage files ("boss dobkeratops")
    bool isBoss(std::string_view name) noexcept;

    // A boss entering from the right edge (std::nullopt: unknown name)
    std::optional<SpawnRequest> boss(std::string_view name);
}
