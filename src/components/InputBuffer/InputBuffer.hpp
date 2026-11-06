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
#include <optional>
#include <array>


namespace RType::Components {
    struct InputBuffer {
        std::array<std::optional<Protecs::Networking::Packets::PlayerInput>, 256> buffer;

        void set(Protecs::Tick tick, const Protecs::Networking::Packets::PlayerInput& input);
        const std::optional<Protecs::Networking::Packets::PlayerInput>& get(Protecs::Tick tick);
    };
}
