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
    struct PickupLook {
        RType::Rules::EntityType type;  // what clients draw
        RType::Rules::Vec2D size;
        float scale;
    };

    constexpr PickupLook pickupLook(RType::Components::Item item) noexcept
    {
        switch (item) {
            case RType::Components::Item::Life:
                return { RType::Rules::EntityCreate::PICKUP_LIFE, RType::Rules::PICKUP_SIZE, RType::Rules::PICKUP_SCALE };
            case RType::Components::Item::Force:
                return { RType::Rules::EntityCreate::PICKUP_FORCE, RType::Rules::PICKUP_FORCE_SIZE, 1.0F };
        }
        return { RType::Rules::EntityCreate::GENERIC, RType::Rules::PICKUP_SIZE, 1.0F };
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
    auto look = pickupLook(loot.item);

    m_Requests->spawns.push_back({
        .type = look.type,
        .position = centre - look.size / 2.0F,
        .size = look.size,
        .velocity = { -Rules::PICKUP_SPEED, 0.0F },
        .scale = look.scale,
        .components = { Components::Pickup{ .item = loot.item } },
    });
}
