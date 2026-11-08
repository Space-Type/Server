/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RandomShots component
*/

#pragma once

namespace RType::Components {
    // Fires a missile to the left at random intervals (small enemies)
    struct RandomShots {
        float min = 0.0F;  // seconds between two shots
        float max = 0.0F;
    };
}
