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


namespace RType::Components {
    using Manager = Wrecks::ECS::ComponentManager<Protecs::Components::Motion,
                                                  Protecs::Components::Transform>;
}
