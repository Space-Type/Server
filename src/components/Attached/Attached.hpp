/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Attached component
*/

#pragma once

#include <protecs/common.hpp>
#include <protecs/utils.hpp>
#include <wrecks/ecs.hpp>


namespace RType::Components {
    // A part of another entity: it follows it, and goes away with it
    struct Attached {
        Wrecks::ECS::Entity parent = 0;
        Protecs::Utils::Vec2D<float> offset = {};  // position from the parent's position
    };
}
