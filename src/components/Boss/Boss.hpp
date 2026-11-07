/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Boss component
*/

#pragma once

#include <cstdint>


namespace RType::Components {
    // Server-only state of a boss body: its bursts of aimed shots
    struct Boss {
        float burstTimer = 1.0F;      // seconds before the next burst
        float shotTimer = 0.0F;       // seconds before the next shot of the current burst
        std::uint8_t shotsLeft = 0;   // shots left in the current burst
    };
}
