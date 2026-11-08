/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** BurstFire system
*/

#include "BurstFire.hpp"
#include <rtype/game.hpp>
#include <chrono>
#include <cmath>
#include <limits>


RType::Systems::BurstFire::BurstFire() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::BurstFire::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& bursts = engine.get<Components::AimedBursts>()[entity];
    const auto& body = engine.get<Components::Body>()[entity];
    float seconds = std::chrono::duration<float>(dt).count();

    if (m_Requests->destroying(entity))
        return;
    if (engine.hasComponent<Components::StopAt>(entity) && body.velocity.x != 0.0F)  // still entering
        return;

    bursts.burstTimer -= seconds;
    if (bursts.shotsLeft == 0 && bursts.burstTimer <= 0.0F) {
        bursts.shotsLeft = bursts.count;
        bursts.shotTimer = 0.0F;
        bursts.burstTimer = bursts.burstInterval;
    }
    if (bursts.shotsLeft == 0)
        return;

    bursts.shotTimer -= seconds;
    if (bursts.shotTimer > 0.0F)
        return;
    bursts.shotTimer = bursts.shotInterval;
    --bursts.shotsLeft;
    shootAtNearestPlayer(engine.get<Protecs::Components::Transform>()[entity].position + bursts.muzzle, bursts.speed);
}

void RType::Systems::BurstFire::shootAtNearestPlayer(Rules::Vec2D from, float speed)
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
        .velocity = direction * (speed / length),
    });
}
