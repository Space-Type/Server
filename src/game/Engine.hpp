/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Engine type
*/

#pragma once

#include <rtype/components.hpp>


namespace RType::Game {
    using Engine = Wrecks::ECS::Engine<std::tuple<>, Components::Manager>;
}
