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

    auto& inputQueue = engine.get<Components::InputBuffer>()[entity];
    auto& input      = engine.get<Protecs::Components::PlayerInput>()[entity];

    for (std::size_t i = 0; i < inputQueue.queued; i++) {
        switch (inputQueue.buffer[i].input) {
            case PlayerInput::UP:      input.up      = inputQueue.buffer[i].state == PlayerInput::HOLD_DOWN; break;
            case PlayerInput::DOWN:    input.down    = inputQueue.buffer[i].state == PlayerInput::HOLD_DOWN; break;
            case PlayerInput::LEFT:    input.left    = inputQueue.buffer[i].state == PlayerInput::HOLD_DOWN; break;
            case PlayerInput::RIGHT:   input.right   = inputQueue.buffer[i].state == PlayerInput::HOLD_DOWN; break;
            case PlayerInput::TRIGGER: input.trigger = inputQueue.buffer[i].state == PlayerInput::HOLD_DOWN; break;
        }
    }

    inputQueue.clear();
}
