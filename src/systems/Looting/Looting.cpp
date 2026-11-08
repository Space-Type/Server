/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Looting system
*/

#include "Looting.hpp"
#include <rtype/game.hpp>


namespace {
    // What clients draw for an item
    constexpr RType::Rules::EntityType pickupType(RType::Components::Item item) noexcept
    {
        switch (item) {
            case RType::Components::Item::Life:
                return RType::Rules::EntityCreate::PICKUP_LIFE;
        }
        return RType::Rules::EntityCreate::GENERIC;
    }
}


RType::Systems::Looting::Looting() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::Looting::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    if (!m_Requests->killed(entity))
        return;

    const auto& loot = engine.get<Components::Loot>()[entity];
    const auto& body = engine.get<Components::Body>()[entity];
    const auto& transform = engine.get<Protecs::Components::Transform>()[entity];

    if (m_Random.between(0.0F, 1.0F) >= m_Chance.value_or(loot.chance))
        return;

    // Centred on the enemy hitbox
    Rules::Vec2D centre = transform.position + body.offset + body.size / 2.0F;

    m_Requests->spawns.push_back({
        .type = pickupType(loot.item),
        .position = centre - Rules::PICKUP_SIZE / 2.0F,
        .size = Rules::PICKUP_SIZE,
        .velocity = { -Rules::PICKUP_SPEED, 0.0F },
        .scale = Rules::PICKUP_SCALE,
        .components = { Components::Pickup{ .item = loot.item } },
    });
}
