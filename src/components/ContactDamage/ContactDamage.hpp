/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ContactDamage component
*/

#pragma once

#include <cstdint>


namespace RType::Components {
    /**
     * Hurts the enemies it touches (the Force): `damage`
     * hit points at most every `interval` seconds. Points
     * for a kill go to its Body owner.
     */
    struct ContactDamage {
        std::uint8_t damage = 1;
        float interval = 0.0F;  // seconds between two hits
        float cooldown = 0.0F;  // seconds before the next hit
    };
}
