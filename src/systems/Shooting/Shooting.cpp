/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Shooting system
*/

#include "Shooting.hpp"
#include <rtype/game.hpp>
#include <algorithm>
#include <chrono>


RType::Systems::Shooting::Shooting() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
    , m_Random()
{

}

void RType::Systems::Shooting::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    using Rules::EntityCreate;

    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& body = engine.get<Components::Body>()[entity];
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;
    float seconds = std::chrono::duration<float>(dt).count();

    body.cooldown = std::max(0.0F, body.cooldown - seconds);
    body.invulnerable = std::max(0.0F, body.invulnerable - seconds);
    if (body.cooldown > 0.0F)
        return;

    if (body.type == EntityCreate::PLAYER) {
        if (body.out || !engine.hasComponent<Protecs::Components::PlayerInput>(entity)
            || !engine.get<Protecs::Components::PlayerInput>()[entity].trigger)
            return;
        body.cooldown = Rules::PLAYER_SHOT_COOLDOWN;
        m_Requests->spawns.push_back({
            .type = EntityCreate::PLAYER_MISSILE,
            .position = { position.x + body.size.x, position.y + (body.size.y - Rules::PLAYER_MISSILE_SIZE.y) / 2.0F },
            .size = Rules::PLAYER_MISSILE_SIZE,
            .velocity = { Rules::PLAYER_MISSILE_SPEED, 0.0F },
            .owner = entity,
        });
    } else if (Rules::isEnemy(body.type)) {
        body.cooldown = m_Random.between(Rules::ENEMY_SHOT_MIN, Rules::ENEMY_SHOT_MAX);
        m_Requests->spawns.push_back({
            .type = EntityCreate::ENEMY_MISSILE,
            .position = { position.x - Rules::ENEMY_MISSILE_SIZE.x, position.y + (body.size.y - Rules::ENEMY_MISSILE_SIZE.y) / 2.0F },
            .size = Rules::ENEMY_MISSILE_SIZE,
            .velocity = { -Rules::ENEMY_MISSILE_SPEED, 0.0F },
            .owner = entity,
        });
    }
}
