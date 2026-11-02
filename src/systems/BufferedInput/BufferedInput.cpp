/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** BufferedInput system
*/

#include "BufferedInput.hpp"
#include <rtype/game.hpp>


RType::Systems::BufferedInput::BufferedInput() noexcept
    : m_Engine(nullptr)
    , m_SimulationTick(nullptr)
{

}

void RType::Systems::BufferedInput::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& buffer = engine.get<Components::InputBuffer>()[entity];
    auto& input  = engine.get<Protecs::Components::PlayerInput>()[entity];

    input = buffer.get(*m_SimulationTick);
}
