/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** EntityPosition struct
*/

#include "EntityPosition.hpp"
#include <rtype/utils.hpp>


void RType::Networking::Packets::EntityPosition::serialize(std::span<std::uint8_t> output) const noexcept
{
    auto* current = output.data();

    RType::Utils::copyFromValue(current, entity);
    RType::Utils::copyFromValue(current, x);
    RType::Utils::copyFromValue(current, y);
}

auto RType::Networking::Packets::EntityPosition::deserialize(std::span<const std::uint8_t> output) noexcept -> EntityPosition
{
    const auto* current = output.data();
    EntityPosition result;

    Utils::copyToValue(result.entity, current);
    Utils::copyToValue(result.x, current);
    Utils::copyToValue(result.y, current);

    return result;
}
