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

    if (!buffer.queuedInput)
        return;

    switch (buffer.queuedInput->input) {
        case PlayerInput::UP:      input.up      = buffer.queuedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::DOWN:    input.down    = buffer.queuedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::LEFT:    input.left    = buffer.queuedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::RIGHT:   input.right   = buffer.queuedInput->state == PlayerInput::HOLD_DOWN; break;
        case PlayerInput::TRIGGER: input.trigger = buffer.queuedInput->state == PlayerInput::HOLD_DOWN; break;
    }

    buffer.queuedInput = {};
}
