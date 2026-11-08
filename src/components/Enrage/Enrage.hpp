/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Enrage component
*/

#pragma once

#include <cstdint>


namespace RType::Components {
    /**
     * A boss weak point that gets angry once its health is low:
     * its BossPhase goes to 2, its AimedBursts get bigger and
     * more frequent, and its swinging parts sweep faster.
     */
    struct Enrage {
        std::uint16_t hpAtMost = 0;    // health that starts phase 2
        std::uint8_t burstCount = 0;   // shots per burst in phase 2
        float burstInterval = 0.0F;    // seconds between two bursts in phase 2
        float swingSpeed = 1.0F;       // the parts sweep that much faster
        bool done = false;
    };
}
