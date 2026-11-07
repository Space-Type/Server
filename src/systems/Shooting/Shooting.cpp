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
    float seconds = std::chrono::duration<float>(dt).count();

    body.cooldown = std::max(0.0F, body.cooldown - seconds);
    body.invulnerable = std::max(0.0F, body.invulnerable - seconds);

    if (body.type == EntityCreate::PLAYER)
        shootAsPlayer(entity, body, seconds);
    else if (Rules::shootsRandomly(body.type) && body.cooldown <= 0.0F)
        shootAsEnemy(entity, body);
}

/**
 * Like R-Type: pressing fires a missile, holding charges
 * (no auto-fire), releasing with a charge of at least
 * level 1 fires a beam. The charge level goes to every
 * client with PlayerStats.
 */
void RType::Systems::Shooting::shootAsPlayer(Wrecks::ECS::Entity entity, Components::Body& body, float seconds)
{
    using Rules::EntityCreate;

    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;
    bool trigger = !body.out && engine.hasComponent<Protecs::Components::PlayerInput>(entity)
                && engine.get<Protecs::Components::PlayerInput>()[entity].trigger;

    if (trigger && !body.triggered) {
        body.charge = 0.0F;
        if (body.cooldown <= 0.0F) {
            body.cooldown = Rules::PLAYER_SHOT_COOLDOWN;
            m_Requests->spawns.push_back({
                .type = EntityCreate::PLAYER_MISSILE,
                .position = { position.x + body.size.x, position.y + (body.size.y - Rules::PLAYER_MISSILE_SIZE.y) / 2.0F },
                .size = Rules::PLAYER_MISSILE_SIZE,
                .velocity = { Rules::PLAYER_MISSILE_SPEED, 0.0F },
                .owner = entity,
                .damage = Rules::MISSILE_DAMAGE,
            });
        }
    } else if (trigger) {
        body.charge += seconds;
    } else if (body.triggered) {
        std::uint8_t level = Rules::chargeLevel(body.charge);

        if (level > 0) {
            Rules::Vec2D size = Rules::BEAM_SIZES[level - 1];

            m_Requests->spawns.push_back({
                .type = Rules::beamType(level),
                .position = { position.x + body.size.x, position.y + (body.size.y - size.y) / 2.0F },
                .size = size,
                .velocity = { Rules::BEAM_SPEED, 0.0F },
                .owner = entity,
                .damage = Rules::beamDamage(level),
            });
        }
        body.charge = 0.0F;
    }
    body.triggered = trigger;

    if (engine.hasComponent<Protecs::Components::PlayerStats>(entity))
        engine.get<Protecs::Components::PlayerStats>()[entity].charge = trigger ? Rules::chargeLevel(body.charge) : 0;
}

void RType::Systems::Shooting::shootAsEnemy(Wrecks::ECS::Entity entity, Components::Body& body)
{
    using Rules::EntityCreate;

    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;

    body.cooldown = m_Random.between(Rules::ENEMY_SHOT_MIN, Rules::ENEMY_SHOT_MAX);
    m_Requests->spawns.push_back({
        .type = EntityCreate::ENEMY_MISSILE,
        .position = { position.x - Rules::ENEMY_MISSILE_SIZE.x, position.y + (body.size.y - Rules::ENEMY_MISSILE_SIZE.y) / 2.0F },
        .size = Rules::ENEMY_MISSILE_SIZE,
        .velocity = { -Rules::ENEMY_MISSILE_SPEED, 0.0F },
        .owner = entity,
    });
}
