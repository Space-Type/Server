/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Collisions system
*/

#include "Collisions.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>
#include <algorithm>
#include <utility>
#include <vector>


RType::Systems::Collisions::Collisions() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

/**
 * The current entity is a player shot or a player: the
 * other bodies are only read (and enemies lose health),
 * the rest goes through the requests.
 */
void RType::Systems::Collisions::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& body = engine.get<Components::Body>()[entity];

    if (m_Requests->destroying(entity))
        return;
    if (body.type == Rules::EntityCreate::PLAYER_MISSILE || Rules::isBeam(body.type))
        hitWithShot(entity);
    else if (body.type == Rules::EntityCreate::PLAYER)
        hitPlayer(entity);
}

/**
 * A missile or a beam takes its damage from the enemies
 * it touches, nearest first. Like R-Type's Wave Cannon, a
 * beam goes through the enemies it kills and the enemy
 * missiles, and stops on the first enemy that survives;
 * a missile stops on the first enemy.
 */
void RType::Systems::Collisions::hitWithShot(Wrecks::ECS::Entity shot)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& body = engine.get<Components::Body>()[shot];
    const auto& position = engine.get<Protecs::Components::Transform>()[shot].position;
    bool beam = Rules::isBeam(body.type);
    std::vector<std::pair<float, Wrecks::ECS::Entity>> touched;

    for (auto [other, unused] : engine.get<Components::Body>()) {
        const auto& otherBody = engine.get<Components::Body>()[other];
        const auto& otherPosition = engine.get<Protecs::Components::Transform>()[other].position;
        bool target = engine.hasComponent<Components::Enemy>(other) || (beam && otherBody.type == Rules::EntityCreate::ENEMY_MISSILE);

        if (target && !m_Requests->destroying(other)
            && Rules::overlap(position + body.offset, body.size, otherPosition + otherBody.offset, otherBody.size))
            touched.emplace_back(otherPosition.x, other);
    }
    std::ranges::sort(touched);

    for (auto [x, other] : touched) {
        auto otherType = engine.get<Components::Body>()[other].type;

        if (otherType == Rules::EntityCreate::ENEMY_MISSILE) {
            m_Requests->destroy(other);
            continue;
        }
        if (engine.hasComponent<Components::Armored>(other)) {  // e.g. a boss body around its weak point
            m_Requests->destroy(shot);
            return;
        }
        if (!damage(other, body.damage)) {
            m_Requests->destroy(shot);
            return;
        }
        if (engine.hasComponent<Protecs::Components::PlayerStats>(body.owner))
            engine.get<Protecs::Components::PlayerStats>()[body.owner].score += engine.get<Components::Enemy>()[other].reward;
        if (!beam) {
            m_Requests->destroy(shot);
            return;
        }
    }
}

/**
 * Enemies and their missiles cost a life to a player that
 * is neither invulnerable nor out: it comes back at its
 * start invulnerable for a moment, or is out with no life
 * left. Clients see the life lost (PlayerStats) before the
 * ship moves back (Transform).
 */
void RType::Systems::Collisions::hitPlayer(Wrecks::ECS::Entity player)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& body = engine.get<Components::Body>()[player];
    auto& transform = engine.get<Protecs::Components::Transform>()[player];

    if (body.out || body.invulnerable > 0.0F || m_Invincible)
        return;

    for (auto [other, unused] : engine.get<Components::Body>()) {
        const auto& otherBody = engine.get<Components::Body>()[other];
        bool hurts = engine.hasComponent<Components::Enemy>(other) || otherBody.type == Rules::EntityCreate::ENEMY_MISSILE;

        if (!hurts || m_Requests->destroying(other)
            || !Rules::overlap(transform.position + body.offset, body.size,
                               engine.get<Protecs::Components::Transform>()[other].position + otherBody.offset, otherBody.size))
            continue;

        auto& stats = engine.get<Protecs::Components::PlayerStats>()[player];

        // A missile goes away; an enemy takes 1 damage (an armored part none)
        if (!engine.hasComponent<Components::Enemy>(other))
            m_Requests->destroy(other);
        else if (!engine.hasComponent<Components::Armored>(other))
            damage(other, 1);
        stats.lives = stats.lives > 0 ? stats.lives - 1 : 0;
        if (stats.lives == 0) {
            logger.info() << "player \"" << engine.get<Components::Player>()[player].name << "\" is out ("
                          << stats.score << " points)" << std::endl;
            body.out = true;
            return;
        }
        body.invulnerable = Rules::RESPAWN_INVULNERABILITY;
        transform.position = Rules::respawnPosition(stats.slot);
        engine.get<Protecs::Components::Velocity>()[player] = {};
        return;
    }
}

/**
 * Takes `amount` hit points from an enemy (one without
 * Health dies at the first hit). Killed: it is destroyed
 * with its Health at 0 (as a kill: it may drop its Loot),
 * and true is returned.
 */
bool RType::Systems::Collisions::damage(Wrecks::ECS::Entity enemy, std::uint8_t amount)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    if (engine.hasComponent<Protecs::Components::Health>(enemy)) {
        auto& health = engine.get<Protecs::Components::Health>()[enemy];

        health.hp = health.hp > amount ? static_cast<std::uint16_t>(health.hp - amount) : 0;
        if (health.hp > 0)
            return false;
    }
    m_Requests->kill(enemy);
    return true;
}
