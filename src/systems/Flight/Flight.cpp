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
#include <cmath>
#include <numbers>


RType::Systems::Flight::Flight() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::Flight::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& body = engine.get<Components::Body>()[entity];

    if (body.type == Rules::EntityCreate::PLAYER)
        return;

    auto& position = engine.get<Protecs::Components::Transform>()[entity].position;
    float seconds = std::chrono::duration<float>(dt).count();

    body.age += seconds;
    position += body.velocity * seconds;
    if (body.pattern == Components::Pattern::Sine)
        position.y = body.baseY + Rules::SINE_AMPLITUDE * std::sin(2.0F * std::numbers::pi_v<float> * body.age / Rules::SINE_PERIOD);
    if (position.x < -Rules::OFFSCREEN_MARGIN || position.x > Protecs::MAP_MAX_X + Rules::OFFSCREEN_MARGIN)
        m_Requests->destroy(entity);
}
