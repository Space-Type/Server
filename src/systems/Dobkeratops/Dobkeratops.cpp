/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Dobkeratops system
*/

#include "Dobkeratops.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>
#include <chrono>
#include <cmath>
#include <limits>


RType::Systems::Dobkeratops::Dobkeratops() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::Dobkeratops::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& boss = engine.get<Components::Boss>()[entity];
    const auto& body = engine.get<Components::Body>()[entity];
    float seconds = std::chrono::duration<float>(dt).count();

    if (m_Requests->destroying(entity))
        return;
    if (!coreAlive()) {
        clearStage(entity);
        return;
    }
    if (body.velocity.x != 0.0F)  // still entering
        return;

    boss.burstTimer -= seconds;
    if (boss.shotsLeft == 0 && boss.burstTimer <= 0.0F) {
        boss.shotsLeft = Rules::BOSS_BURST;
        boss.shotTimer = 0.0F;
        boss.burstTimer = Rules::BOSS_BURST_INTERVAL;
    }
    if (boss.shotsLeft == 0)
        return;

    boss.shotTimer -= seconds;
    if (boss.shotTimer > 0.0F)
        return;
    boss.shotTimer = Rules::BOSS_SHOT_INTERVAL;
    --boss.shotsLeft;
    shootAtNearestPlayer(engine.get<Protecs::Components::Transform>()[entity].position + Rules::BOSS_MOUTH * Rules::BOSS_SCALE);
}

bool RType::Systems::Dobkeratops::coreAlive() const
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    for (auto [entity, body] : engine.get<Components::Body>())
        if (body.type == Rules::EntityCreate::ENEMY_DOBKERATOPS_CORE && !m_Requests->destroying(entity))
            return true;
    return false;
}

/**
 * The core is dead: the boss, the enemies and their shots go
 * away, and the level is cleared (no more waves nor boss).
 */
void RType::Systems::Dobkeratops::clearStage(Wrecks::ECS::Entity boss)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    m_Requests->destroy(boss);
    for (auto [entity, body] : engine.get<Components::Body>())
        if (Rules::isEnemy(body.type) || body.type == Rules::EntityCreate::ENEMY_MISSILE)
            m_Requests->destroy(entity);
    for (auto [entity, state] : engine.get<Components::LevelState>())
        state.cleared = true;
    logger.info() << "stage 1 cleared" << std::endl;
}

void RType::Systems::Dobkeratops::shootAtNearestPlayer(Rules::Vec2D from)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    Rules::Vec2D target = { 0.0F, from.y };
    float nearest = std::numeric_limits<float>::max();

    for (auto [entity, body] : engine.get<Components::Body>()) {
        if (body.type != Rules::EntityCreate::PLAYER || body.out)
            continue;

        Rules::Vec2D player = engine.get<Protecs::Components::Transform>()[entity].position + body.size / 2.0F;
        float distance = std::hypot(player.x - from.x, player.y - from.y);

        if (distance < nearest) {
            nearest = distance;
            target = player;
        }
    }

    Rules::Vec2D direction = { target.x - from.x, target.y - from.y };
    float length = std::hypot(direction.x, direction.y);

    if (length <= 0.0F)
        return;
    m_Requests->spawns.push_back({
        .type = Rules::EntityCreate::ENEMY_MISSILE,
        .position = from,
        .size = Rules::ENEMY_MISSILE_SIZE,
        .velocity = direction * (Rules::BOSS_SHOT_SPEED / length),
    });
}
