/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Loot component
*/

#pragma once

#include "../Pickup/Pickup.hpp"


namespace RType::Components {
    /**
     * An enemy that may drop a bonus (a Pickup) where it
     * dies, when a player kills it. Leaving the screen or
     * the end of a stage drops nothing.
     */
    struct Loot {
        Item item = Item::Life;
        float chance = 0.0F;  // 0 (never) to 1 (always)
    };
}
