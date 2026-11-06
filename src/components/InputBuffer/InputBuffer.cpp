/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** InputBuffer component
*/

#include "InputBuffer.hpp"


void RType::Components::InputBuffer::set(Protecs::Tick tick, const Protecs::Networking::Packets::PlayerInput& input)
{
    buffer[tick % buffer.size()] = input;
}

auto RType::Components::InputBuffer::get(Protecs::Tick tick) -> const std::optional<Protecs::Networking::Packets::PlayerInput>&
{
    return buffer[tick % buffer.size()];
}
