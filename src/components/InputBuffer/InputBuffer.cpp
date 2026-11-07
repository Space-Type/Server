/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** InputBuffer component
*/

#include "InputBuffer.hpp"


void RType::Components::InputBuffer::push(const Protecs::Networking::Packets::PlayerInput& input) noexcept
{
    if (queued == buffer.size())
        return;

    buffer[queued] = input;
    queued++;
}

void RType::Components::InputBuffer::clear() noexcept
{
    queued = 0;
}
