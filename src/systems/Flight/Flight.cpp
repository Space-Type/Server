/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Flight system
*/

#include "Flight.hpp"
#include <rtype/game.hpp>
#include <chrono>


RType::Systems::Flight::Flight() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::Flight::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& body = engine.get<Components::Body>()[entity];

    if (body.type == Rules::EntityCreate::PLAYER)
        return;

    auto& position = engine.get<Protecs::Components::Transform>()[entity].position;

    position += body.velocity * std::chrono::duration<float>(dt).count();
    if (position.x < -Rules::OFFSCREEN_MARGIN || position.x > Protecs::MAP_MAX_X + Rules::OFFSCREEN_MARGIN)
        m_Requests->destroy(entity);
}
