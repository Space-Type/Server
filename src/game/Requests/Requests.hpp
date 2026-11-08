/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** entity requests made by the systems
*/

#pragma once

#include <rtype/components.hpp>
#include <protecs/common.hpp>
#include <protecs/components.hpp>
#include <protecs/networking.hpp>
#include <wrecks/ecs.hpp>
#include <cstdint>
#include <variant>
#include <vector>


namespace RType::Game {
    // Components a spawned entity may get besides its Body and Transform (Health is shared with the clients)
    using ExtraComponent = std::variant<Protecs::Components::Health,
                                        Components::Enemy,
                                        Components::Armored,
                                        Components::Boss,
                                        Components::StopAt,
                                        Components::RandomShots,
                                        Components::AimedBursts,
                                        Components::Swing,
                                        Components::Enrage,
                                        Components::Loot,
                                        Components::Pickup,
                                        Components::Force,
                                        Components::ContactDamage,
                                        Components::Shield,
                                        Protecs::Components::BossPhase>;

    /**
     * An entity a system wants to create: its Body, its
     * Transform, its other components, and its parts.
     */
    struct SpawnRequest {
        Protecs::Networking::Packets::EntityCreate::EntityType type;
        Protecs::Utils::Vec2D<float> position;  // a part: from its parent's position
        Protecs::Utils::Vec2D<float> size;
        Protecs::Utils::Vec2D<float> velocity = {};
        Wrecks::ECS::Entity owner = 0;  // player who shot a missile (score)
        float cooldown = 0.0F;          // seconds before its first shot
        std::uint8_t damage = 0;        // missiles and beams
        Components::Pattern pattern = Components::Pattern::Straight;
        Protecs::Utils::Vec2D<float> offset = {};  // hitbox position from the entity position
        float scale = 1.0F;             // Transform.scale: clients draw the sprite that much bigger
        std::vector<ExtraComponent> components = {};
        std::vector<SpawnRequest> parts = {};  // created after it, Attached to it
    };

    /**
     * Entities to create and delete, asked by the systems.
     *
     * Systems run while Wrecks goes through the entities:
     * creating or deleting one then would break that loop.
     * The Game applies the requests after each fixed update,
     * and tells the clients (EntityCreate / EntityDelete).
     */
    struct Requests {
        std::vector<SpawnRequest> spawns;
        std::vector<Wrecks::ECS::Entity> destroys;
        std::vector<Wrecks::ECS::Entity> kills;  // destroyed by a player (not gone off screen): they may drop their Loot

        void destroy(Wrecks::ECS::Entity entity);
        void kill(Wrecks::ECS::Entity entity);
        [[ nodiscard ]] bool killed(Wrecks::ECS::Entity entity) const noexcept;
        [[ nodiscard ]] bool destroying(Wrecks::ECS::Entity entity) const noexcept;
    };
}
