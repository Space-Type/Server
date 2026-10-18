/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ComponentManager type
*/

#pragma once

#include <rtype/components/types.hpp>
#include <protecs/components.hpp>
#include <wrecks/ecs.hpp>
#include <wrecks/utils.hpp>


namespace RType::Components {
    using Manager = Wrecks::Utils::concat_types_t<
        Protecs::Components::Manager,
        Wrecks::ECS::ComponentManager<>
    >;
}
