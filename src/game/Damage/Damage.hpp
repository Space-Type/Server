/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** damage taken by enemies, shared by the systems
*/

#pragma once

#include "../Engine.hpp"
#include "../Requests/Requests.hpp"
#include <cstdint>


namespace RType::Game {
    /**
     * Takes `amount` hit points from an enemy (one without
     * Health dies at the first hit). Killed: it is destroyed
     * with its Health at 0 (as a kill: it may drop its Loot),
     * and true is returned. Include it from .cpp files only.
     */
    bool damage(Engine& engine, Requests& requests, Wrecks::ECS::Entity enemy, std::uint8_t amount);
}
