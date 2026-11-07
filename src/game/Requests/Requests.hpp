/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** entity requests made by the systems
*/

#pragma once

#include <protecs/common.hpp>
#include <protecs/networking.hpp>
#include <wrecks/ecs.hpp>
#include <vector>


namespace RType::Game {
    // An enemy or a missile a system wants to create
    struct SpawnRequest {
        Protecs::Networking::Packets::EntityCreate::EntityType type;
        Protecs::Utils::Vec2D<float> position;
        Protecs::Utils::Vec2D<float> size;
        Protecs::Utils::Vec2D<float> velocity;
        Wrecks::ECS::Entity owner = 0;  // player who shot a missile (score)
        float cooldown = 0.0F;          // seconds before its first shot
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

        void destroy(Wrecks::ECS::Entity entity);
        [[ nodiscard ]] bool destroying(Wrecks::ECS::Entity entity) const noexcept;
    };
}
