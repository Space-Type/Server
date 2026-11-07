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


namespace RType::Components {
    struct InputBuffer {
        std::optional<Protecs::Networking::Packets::PlayerInput> queuedInput;
    };
}
