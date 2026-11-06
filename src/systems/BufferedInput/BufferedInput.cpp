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
    using Protecs::Networking::Packets::PlayerInput;

    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& buffer = engine.get<Components::InputBuffer>()[entity];
    auto& input  = engine.get<Protecs::Components::PlayerInput>()[entity];

    const auto& bufferedInput = buffer.get(*m_SimulationTick);
    if (!bufferedInput)
        return;

    switch (bufferedInput->input) {
        case PlayerInput::UP:      input.up      = bufferedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::DOWN:    input.down    = bufferedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::LEFT:    input.left    = bufferedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::RIGHT:   input.right   = bufferedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::TRIGGER: input.trigger = bufferedInput->state == PlayerInput::HOLD_DOWN; break;
    }
}
