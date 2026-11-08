/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Force component
*/

#pragma once

#include <cstdint>


namespace RType::Components {
    /**
     * R-Type's Force: a pod of a player (its Body owner),
     * attached to the front or the back of the ship, or
     * launched away and recalled with the Force button.
     * It is lost with a life of its player.
     */
    struct Force {
        enum class State : std::uint8_t {
            Attached,
            Launched,   // flying away from the ship
            Floating,   // launched, then stopped: it stays there
            Returning,  // recalled: flying back to the ship
        };

        enum class Side : std::uint8_t {
            Front,
            Back,
        };

        std::uint8_t level = 1;      // 1 to Rules::FORCE_LEVELS
        State state = State::Attached;
        Side side = Side::Front;
        float travelled = 0.0F;      // world units flown since it was launched
        bool pressed = false;        // the Force button was held at the previous step
        std::uint8_t lives = 0;      // lives of its player when it got it: one less, and the Force is lost
    };
}
