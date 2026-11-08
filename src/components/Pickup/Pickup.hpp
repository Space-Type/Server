/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Pickup component
*/

#pragma once

#include <cstdint>


namespace RType::Components {
    // What a bonus gives to the player who takes it
    enum class Item : std::uint8_t {
        Life,  // one more life, up to Rules::MAX_LIVES
    };

    /**
     * A bonus flying in the level: the player who touches
     * it, or whose shot touches it, gets its item.
     */
    struct Pickup {
        Item item = Item::Life;
    };
}
