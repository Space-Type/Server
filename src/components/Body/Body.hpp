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
#include <cstdint>


namespace RType::Components {
    // How an enemy flies (stage files: "straight", "sine")
    enum class Pattern : std::uint8_t {
        Straight,
        Sine,      // a wave around its starting height, like the Pata-Pata
    };

    /**
     * Server-only gameplay data of a player, an enemy
     * or a missile. Positions stay in Transform (shared),
     * the hitbox starts at its top-left corner.
     */
    struct Body {
        Protecs::Networking::Packets::EntityCreate::EntityType type = Protecs::Networking::Packets::EntityCreate::GENERIC;
        Protecs::Utils::Vec2D<float> size = {};      // hitbox, world units
        Protecs::Utils::Vec2D<float> offset = {};    // hitbox position from the Transform position
        Protecs::Utils::Vec2D<float> velocity = {};  // world units per second (players move with their own systems)
        Wrecks::ECS::Entity owner = 0;               // player who shot a missile (score)
        float cooldown = 0.0F;                       // seconds before the next shot
        float invulnerable = 0.0F;                   // seconds left after a player respawns
        bool out = false;                            // a player with no life left: no more shots nor hits
        float charge = 0.0F;                         // seconds the trigger has been held (charged shot)
        bool triggered = false;                      // trigger held at the previous step
        std::uint8_t damage = 0;                     // missiles and beams: hit points taken from an enemy
        Pattern pattern = Pattern::Straight;
        float baseY = 0.0F;                          // height the wave goes around (Pattern::Sine)
        float age = 0.0F;                            // seconds since it appeared
    };
}
