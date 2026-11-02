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


namespace RType::Components {
    struct InputBuffer : Wrecks::Utils::SequenceContainer<Protecs::Components::PlayerInput> {};
}
