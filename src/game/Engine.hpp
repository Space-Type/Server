/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Engine type
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/systems.hpp>


namespace RType::Game {
    using Engine = Wrecks::ECS::Engine<Systems::Tuple, Components::Manager>;
}
