/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Shielding system
*/

#include "Shielding.hpp"
#include <rtype/game.hpp>


RType::Systems::Shielding::Shielding() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::Shielding::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& body = engine.get<Components::Body>()[entity];
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;

    if (m_Requests->destroying(entity))
        return;
    for (auto [other, otherBody] : engine.get<Components::Body>())
        if (otherBody.type == Rules::EntityCreate::ENEMY_MISSILE
            && Rules::overlap(position + body.offset, body.size, engine.get<Protecs::Components::Transform>()[other].position + otherBody.offset, otherBody.size))
            m_Requests->destroy(other);
}
