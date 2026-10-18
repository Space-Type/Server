/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** PlayerInput struct
*/

#pragma once

#include <cstdint>
#include <span>


namespace RType::Networking::Packets {
    struct PlayerInput {
        enum : std::uint8_t { UP, DOWN, LEFT, RIGHT, SHOOT } input;
        enum : std::uint8_t { HOLD_DOWN, RELEASE } state;

        void serialize(std::span<std::uint8_t> output) const noexcept;
        static PlayerInput deserialize(std::span<const std::uint8_t> input) noexcept;
    };
}
