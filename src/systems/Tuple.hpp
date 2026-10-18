/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Systems type
*/

#pragma once

#include <rtype/systems/types.hpp>
#include <protecs/systems.hpp>
#include <wrecks/utils.hpp>
#include <tuple>


namespace RType::Systems {
    using Tuple = Wrecks::Utils::concat_types_t<
        Protecs::Systems::Tuple,
        std::tuple<>
    >;
}
