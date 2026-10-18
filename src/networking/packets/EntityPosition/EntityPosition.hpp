/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** EntityPosition struct
*/

#pragma once

#include <wrecks.hpp>
#include <span>


namespace RType::Networking::Packets {
    struct EntityPosition {
        Wrecks::ECS::Entity entity;
        float x;
        float y;

        void serialize(std::span<std::uint8_t> output) const noexcept;
        static EntityPosition deserialize(std::span<const std::uint8_t> input) noexcept;
    };
}
