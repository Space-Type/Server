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


RType::Systems::Collisions::Collisions() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

/**
 * The current entity is a player missile or a player:
 * the other bodies are only read, the changes are the
 * current entity's (and its shooter's score) and requests.
 */
void RType::Systems::Collisions::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    using Rules::EntityCreate;

    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& body = engine.get<Components::Body>()[entity];
    auto& transform = engine.get<Protecs::Components::Transform>()[entity];

    bool isMissile = body.type == EntityCreate::PLAYER_MISSILE;
    bool isTarget = body.type == EntityCreate::PLAYER && !body.out && body.invulnerable <= 0.0F;

    if ((!isMissile && !isTarget) || m_Requests->destroying(entity))
        return;

    for (auto [other, unused] : engine.get<Components::Body>()) {
        const auto& otherBody = engine.get<Components::Body>()[other];
        bool hits = isMissile ? Rules::isEnemy(otherBody.type)
                              : Rules::isEnemy(otherBody.type) || otherBody.type == EntityCreate::ENEMY_MISSILE;

        if (other == entity || !hits || m_Requests->destroying(other)
            || !Rules::overlap(transform.position, body.size, engine.get<Protecs::Components::Transform>()[other].position, otherBody.size))
            continue;

        m_Requests->destroy(other);
        if (isMissile) {
            m_Requests->destroy(entity);
            if (engine.hasComponent<Protecs::Components::PlayerStats>(body.owner))
                engine.get<Protecs::Components::PlayerStats>()[body.owner].score += Rules::ENEMY_SCORE;
            return;
        }

        // A hit player: the clients see the life lost (PlayerStats) before the ship moves back (Transform)
        auto& stats = engine.get<Protecs::Components::PlayerStats>()[entity];

        stats.lives = stats.lives > 0 ? stats.lives - 1 : 0;
        if (stats.lives == 0) {
            logger.info() << "player \"" << engine.get<Components::Player>()[entity].name << "\" is out ("
                          << stats.score << " points)" << std::endl;
            body.out = true;
            return;
        }
        body.invulnerable = Rules::RESPAWN_INVULNERABILITY;
        transform.position = Rules::respawnPosition(stats.slot);
        engine.get<Protecs::Components::Velocity>()[entity] = {};
        return;
    }
}
