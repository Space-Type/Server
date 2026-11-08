/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Enemy component
*/

#pragma once

#include <cstdint>


namespace RType::Components {
    /**
     * An enemy part: player shots and beams hit it, it costs a
     * life to a player who touches it, and it goes away when a
     * stage is cleared. Enemy missiles are not enemies.
     */
    struct Enemy {
        std::uint32_t reward = 0;  // points for the player who kills it
    };
}
