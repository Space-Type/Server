/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Swing component
*/

#pragma once

#include <protecs/common.hpp>
#include <protecs/utils.hpp>


namespace RType::Components {
    /**
     * A part that sweeps an arc around a point of its parent
     * (Attached), like a segment of a tail: at `length` from
     * `anchor`, in the direction
     *
     *     baseAngle + amplitude * sin(2 pi * time / period - lag)
     *
     * Segments with a growing length and lag make a whip.
     */
    struct Swing {
        Protecs::Utils::Vec2D<float> anchor = {};  // from the parent's position
        float length = 0.0F;      // world units from the anchor
        float baseAngle = 0.0F;   // radians, 0 to the right, pi / 2 downwards
        float amplitude = 0.0F;   // radians on each side of baseAngle
        float period = 1.0F;      // seconds per sweep
        float lag = 0.0F;         // radians behind the start of the tail
        float speed = 1.0F;       // the sweep runs that much faster (Enrage)
        float time = 0.0F;        // seconds of sweep, scaled by speed
    };
}
