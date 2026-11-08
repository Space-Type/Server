/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** AimedBursts component
*/

#pragma once

#include <protecs/common.hpp>
#include <protecs/utils.hpp>
#include <cstdint>


namespace RType::Components {
    /**
     * Fires bursts of shots aimed at the nearest player. With
     * StopAt, it waits until it has stopped.
     */
    struct AimedBursts {
        std::uint8_t count = 1;                    // shots per burst
        float shotInterval = 0.0F;                 // seconds between the shots of a burst
        float burstInterval = 0.0F;                // seconds between two bursts
        float speed = 0.0F;                        // world units per second
        Protecs::Utils::Vec2D<float> muzzle = {};  // where shots leave, from its position

        float burstTimer = 1.0F;                   // seconds before the next burst
        float shotTimer = 0.0F;                    // seconds before the next shot of the burst
        std::uint8_t shotsLeft = 0;                // shots left in the current burst
    };
}
