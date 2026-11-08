/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Swinging system
*/

#include "Swinging.hpp"
#include <rtype/game.hpp>
#include <chrono>
#include <cmath>
#include <numbers>


RType::Systems::Swinging::Swinging() noexcept
    : m_Engine(nullptr)
{

}

void RType::Systems::Swinging::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& swing = engine.get<Components::Swing>()[entity];
    auto& attached = engine.get<Components::Attached>()[entity];

    swing.time += std::chrono::duration<float>(dt).count() * swing.speed;

    float angle = swing.baseAngle
                + swing.amplitude * std::sin(2.0F * std::numbers::pi_v<float> * swing.time / swing.period - swing.lag);

    attached.offset = swing.anchor + Protecs::Utils::Vec2D<float>{ std::cos(angle), std::sin(angle) } * swing.length;
}
