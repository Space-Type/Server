/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** PlayerInput struct
*/

#include "PlayerInput.hpp"
#include <rtype/utils.hpp>


void RType::Networking::Packets::PlayerInput::serialize(std::span<std::uint8_t> output) const noexcept
{
    auto* current = output.data();

    RType::Utils::copyFromValue(current, input);
    RType::Utils::copyFromValue(current, state);
}

auto RType::Networking::Packets::PlayerInput::deserialize(std::span<const std::uint8_t> output) noexcept -> PlayerInput
{
    const auto* current = output.data();
    PlayerInput result;

    Utils::copyToValue(result.input, current);
    Utils::copyToValue(result.state, current);

    return result;
}
