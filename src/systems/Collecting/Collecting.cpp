/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Collecting system
*/

#include "Collecting.hpp"
#include <rtype/game.hpp>
#include <algorithm>


RType::Systems::Collecting::Collecting() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

/**
 * The first player or player shot touching the pickup
 * takes it.
 */
void RType::Systems::Collecting::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& body = engine.get<Components::Body>()[entity];
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;

    if (m_Requests->destroying(entity))
        return;

    for (auto [other, otherBody] : engine.get<Components::Body>()) {
        bool shot = otherBody.type == Rules::EntityCreate::PLAYER_MISSILE || Rules::isBeam(otherBody.type);
        auto player = shot ? otherBody.owner : other;

        if ((!shot && otherBody.type != Rules::EntityCreate::PLAYER) || m_Requests->destroying(other)
            || !Rules::overlap(position + body.offset, body.size,
                               engine.get<Protecs::Components::Transform>()[other].position + otherBody.offset, otherBody.size))
            continue;
        if (!engine.hasComponent<Protecs::Components::PlayerStats>(player) || engine.get<Components::Body>()[player].out)
            continue;

        give(engine.get<Components::Pickup>()[entity].item, player);
        if (otherBody.type == Rules::EntityCreate::PLAYER_MISSILE)
            m_Requests->destroy(other);
        m_Requests->destroy(entity);
        return;
    }
}

/**
 * Clients see the change through PlayerStats. A life over
 * Rules::MAX_LIVES is lost: the pickup is taken anyway.
 */
void RType::Systems::Collecting::give(Components::Item item, Wrecks::ECS::Entity player)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& stats = engine.get<Protecs::Components::PlayerStats>()[player];

    switch (item) {
        case Components::Item::Life:
            stats.lives = std::min<std::uint8_t>(static_cast<std::uint8_t>(stats.lives + 1), Rules::MAX_LIVES);
            break;
    }
}
