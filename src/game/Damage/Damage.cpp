/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** damage taken by enemies, shared by the systems
*/

#include "Damage.hpp"


bool RType::Game::damage(Engine& engine, Requests& requests, Wrecks::ECS::Entity enemy, std::uint8_t amount)
{
    if (engine.hasComponent<Protecs::Components::Health>(enemy)) {
        auto& health = engine.get<Protecs::Components::Health>()[enemy];

        health.hp = health.hp > amount ? static_cast<std::uint16_t>(health.hp - amount) : 0;
        if (health.hp > 0)
            return false;
    }
    requests.kill(enemy);
    return true;
}
