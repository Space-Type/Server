/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** InputBuffer component
*/

#pragma once

#include <protecs/networking.hpp>
#include <wrecks/utils.hpp>
#include <array>


namespace RType::Components {
    struct InputBuffer {
        std::array<Protecs::Networking::Packets::PlayerInput, 50> buffer;
        std::size_t queued;

        void push(const Protecs::Networking::Packets::PlayerInput& input) noexcept;
        void clear() noexcept;
    };
}
