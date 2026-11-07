/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Body component
*/

#pragma once

#include <protecs/common.hpp>
#include <protecs/networking.hpp>


namespace RType::Components {
    /**
     * Server-only gameplay data of a player, an enemy
     * or a missile. Positions stay in Transform (shared),
     * the hitbox starts at its top-left corner.
     */
    struct Body {
        Protecs::Networking::Packets::EntityCreate::EntityType type = Protecs::Networking::Packets::EntityCreate::GENERIC;
        Protecs::Utils::Vec2D<float> size = {};      // hitbox, world units
        Protecs::Utils::Vec2D<float> velocity = {};  // world units per second (players move with their own systems)
        Wrecks::ECS::Entity owner = 0;               // player who shot a missile (score)
        float cooldown = 0.0F;                       // seconds before the next shot
        float invulnerable = 0.0F;                   // seconds left after a player respawns
    };
}
